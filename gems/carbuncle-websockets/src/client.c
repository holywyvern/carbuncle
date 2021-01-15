#include "carbuncle/client.h"

#include <libwebsockets.h>
#include <physfs.h>

#include <mruby/array.h>
#include <mruby/data.h>
#include <mruby/class.h>
#include <mruby/variable.h>

#if defined(_WIN32) || defined(WIN32)
#include <io.h>
#else
#include <fcntl.h>
#endif

#include "carbuncle/ws_files.h"

#define CLIENT_INITIALIZE_KEY_SIZE 2
#define CLIENT_BUFFER_SIZE 8

#define OPEN_SYMBOL mrb_intern_cstr(mrb, "#open")
#define QUEUE_SYMBOL mrb_intern_cstr(mrb, "#queue")

const char *CLIENT_KEYS[CLIENT_INITIALIZE_KEY_SIZE] = {
  "tls", "cert"
};

struct carbuncle_message
{
  size_t                    size;
  char                     *value;
  struct carbuncle_message *next;
};

struct carbuncle_client_state
{
  mrb_state *mrb;
  mrb_value  self;
  struct carbuncle_message *messages;
  struct carbuncle_message *last_message;
};

struct carbuncle_ws
{
  struct lws_context *ctx;
  struct lws *ws;
};

static void
free_messages(mrb_state *mrb, struct carbuncle_message *root)
{
 while (root)
 {
   struct carbuncle_message *msg = root;
   root = root->next;
   mrb_free(mrb, msg->value);
   mrb_free(mrb, msg);
 } 
}

static void
mrb_client_free(mrb_state *mrb, void *ptr)
{
  if (ptr)
  {
    struct carbuncle_ws *data = ptr;
    struct carbuncle_client_state *state = lws_context_user(data->ctx);
    free_messages(mrb, state->messages);
    mrb_free(mrb, state);
    lws_context_destroy(data->ctx);
    mrb_free(mrb, data);
  }
}

static int
mrb_server_callback( struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len )
{
  struct carbuncle_client_state *state;
  mrb_value self;
  mrb_state *mrb;
  state = user;
  mrb = state->mrb;
  self = state->self;
	switch (reason)
	{
    case LWS_CALLBACK_CLIENT_ESTABLISHED:
    {
      mrb_iv_set(mrb, self, OPEN_SYMBOL, mrb_true_value());
      break;
    }
    case LWS_CALLBACK_CLIENT_RECEIVE:
    {
      mrb_ary_push(mrb, mrb_iv_get(mrb, self, QUEUE_SYMBOL), mrb_str_new(mrb, in, len));
      break;
    }
    case LWS_CALLBACK_CLIENT_WRITEABLE:
    {
      if (state->messages)
      {
        struct carbuncle_message *msg = state->messages;
        state->messages = msg->next;
        lws_write(wsi, &(msg->value[LWS_PRE]), msg->size, LWS_WRITE_TEXT);
        mrb_free(mrb, msg->value);
        mrb_free(mrb, msg);
      }
      break;
    }
    case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
    {
      struct RClass *carbuncle = mrb_module_get(mrb, "Carbuncle");
      mrb_iv_set(mrb, self, OPEN_SYMBOL, mrb_false_value());
      mrb_raise(mrb, mrb_class_get_under(mrb, carbuncle, "ConnectionError"), "Connection error.");
      break;
    }
		case LWS_CALLBACK_CLOSED:
    {
      mrb_iv_set(mrb, self, OPEN_SYMBOL, mrb_false_value());
      break;
    }
    default: break;
  }
  return 0;
}

static const struct mrb_data_type client_data_type = {
  "Carbuncle::Server", mrb_client_free
};

static struct lws_protocols protocols[] =
{
	{
		"carbuncle-protocol",
		mrb_server_callback,
		0,
		CLIENT_BUFFER_SIZE,
	},
	{ NULL, NULL, 0, 0 } /* terminator */
};

static inline void
send_message(mrb_state *mrb, struct carbuncle_client_state *ctx, const char *txt, size_t size)
{
  struct carbuncle_message *prev;
  struct carbuncle_message *msg = mrb_malloc(mrb, sizeof *msg);
  memset(msg, 0, sizeof *msg);
  msg->size = size;
  msg->value = mrb_malloc(mrb, LWS_PRE + LWS_SEND_BUFFER_POST_PADDING + size);
  memcpy(&(msg->value[LWS_PRE]), txt, size);
  if (!ctx->messages)
  {
    ctx->messages = msg;
    ctx->last_message = msg;
    return;
  }
  ctx->last_message->next = msg;
  ctx->last_message = msg;
}

static inline struct carbuncle_ws *
get_context(mrb_state *mrb, mrb_value self)
{
  return DATA_GET_PTR(mrb, self, &client_data_type, struct carbuncle_ws);
}

static mrb_value
mrb_client_initialize(mrb_state *mrb, mrb_value self)
{
  struct lws_plat_file_ops *fops;
  const char *path, *origin;
  struct carbuncle_ws *data;
  struct lws_client_connect_info ccinfo;
  struct carbuncle_client_state *state;
  struct lws_context_creation_info info;
  struct lws_context *ctx;
  struct lws *ws;
  mrb_int port;
  const char *host;
  mrb_value kw_values[CLIENT_INITIALIZE_KEY_SIZE];
  const mrb_kwargs kwargs = { CLIENT_INITIALIZE_KEY_SIZE, kw_values, CLIENT_KEYS, 0, NULL };
  port = 0;
  mrb_get_args(mrb, ":zi|z", &kwargs, &host, &port, &path);
  mrb_iv_set(mrb, self, QUEUE_SYMBOL, mrb_ary_new(mrb));
  data = mrb_malloc(mrb, sizeof *data);
  state = mrb_malloc(mrb, sizeof *state);
  state->mrb = mrb;
  state->self = self;
  state->messages = NULL;
  memset(&info, 0, sizeof info);
  memset(&ccinfo, 0, sizeof ccinfo);
  info.gid = -1;
  info.uid = -1;
  info.user = state;
  ctx = lws_create_context(&info);
  fops = lws_get_fops(ctx);
  fops->LWS_FOP_CLOSE = close_ws_file;
  fops->LWS_FOP_OPEN = open_ws_file;
  fops->LWS_FOP_READ = read_ws_file;
  fops->LWS_FOP_SEEK_CUR = seek_cur_ws_file;
  fops->LWS_FOP_WRITE = write_ws_file;
  lws_set_fops(ctx, fops);
  ccinfo.context = ctx;
  ccinfo.address = host;
  ccinfo.port = port;
  ccinfo.userdata = state;
  ccinfo.path = path == NULL ? "/" : path;
  ccinfo.origin = origin == NULL ? origin : "origin";
  ccinfo.protocol = protocols[0].name;
  ccinfo.host = lws_canonical_hostname(ctx);
  data->ctx = ctx;
  data->ws = lws_client_connect_via_info(&ccinfo);
  DATA_PTR(self) = ctx;
  DATA_TYPE(self) = &client_data_type;
  return self;
}

static mrb_value
mrb_client_send(mrb_state *mrb, mrb_value self)
{
  mrb_int size;
  char *msg;
  struct carbuncle_ws *ctx = get_context(mrb, self);
  mrb_get_args(mrb, "s", &msg, &size);
  send_message(mrb, lws_context_user(ctx->ctx), msg, size);
  if (ctx->ws)
  {
    lws_callback_on_writable(ctx->ws);
  }
  return self;
}

static mrb_value
mrb_client_recv(mrb_state *mrb, mrb_value self)
{
  mrb_float timeout = 0;
  struct carbuncle_ws *ctx = get_context(mrb, self);
  mrb_get_args(mrb, "|f", &timeout);
  lws_service(ctx->ctx, timeout * 1000);
  return self;
}

static mrb_value
mrb_client_pop(mrb_state *mrb, mrb_value self)
{
  return mrb_ary_pop(mrb, mrb_iv_get(mrb, self, QUEUE_SYMBOL));
}

static mrb_value
mrb_client_openQ(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, OPEN_SYMBOL);
}

static mrb_value
mrb_client_emptyQ(mrb_state *mrb, mrb_value self)
{
  mrb_value ary = mrb_iv_get(mrb, self, QUEUE_SYMBOL);
  return mrb_bool_value(RARRAY_LEN(ary));
}


static mrb_value
mrb_client_close(mrb_state *mrb, mrb_value self)
{
  struct carbuncle_ws *ctx = get_context(mrb, self);
  mrb_client_free(mrb, ctx);
  DATA_PTR(self) = NULL;
  mrb_iv_set(mrb, self, OPEN_SYMBOL, mrb_false_value());
  return self;
}

void
mrb_init_carbuncle_client(mrb_state *mrb)
{
  struct RClass *carbuncle = mrb_module_get(mrb, "Carbuncle");
  struct RClass *ws = mrb_define_class_under(mrb, carbuncle, "Client", mrb->object_class);
  MRB_SET_INSTANCE_TT(ws, MRB_TT_DATA);

  mrb_define_method(mrb, ws, "initialize", mrb_client_initialize, MRB_ARGS_REQ(2)|MRB_ARGS_KEY(CLIENT_INITIALIZE_KEY_SIZE, 0));

  mrb_define_method(mrb, ws, "send", mrb_client_send, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, ws, "recv", mrb_client_recv, MRB_ARGS_OPT(1));
  mrb_define_method(mrb, ws, "update", mrb_client_recv, MRB_ARGS_OPT(1));
  mrb_define_method(mrb, ws, "pop", mrb_client_pop, MRB_ARGS_NONE());

  mrb_define_method(mrb, ws, "open?", mrb_client_openQ, MRB_ARGS_NONE());
  mrb_define_method(mrb, ws, "empty?", mrb_client_emptyQ, MRB_ARGS_NONE());

  mrb_define_method(mrb, ws, "close", mrb_client_close, MRB_ARGS_NONE());
}
