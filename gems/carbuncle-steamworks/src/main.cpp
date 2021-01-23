#include <mruby.h>
#include <mruby/error.h>

#include <stdbool.h>
#include <stdlib.h>

#include <carbuncle/core.h>
#include <carbuncle/steamworks.h>

#ifdef _WIN32
# include <windows.h>
# define LIBRARY_NAME "steam_api.dll"
typedef HMODULE LibraryHandle;
typedef FARPROC LibraryFunction;
#else
# include <dlfcn.h>
typedef void* LibraryHandle;
typedef void* LibraryFunction;

# ifdef __unix__
#   define LIBRARY_NAME "libsteam_api.so"
# elif __APPLE__
#   define LIBRARY_NAME "libsteam_api.dylib"
# endif
#endif

static LibraryHandle mrb_carbuncle_steamworks_handle;

static bool (*steamworks_Init)(void);
static bool (*steamworks_RestartAppIfNecessary)(uint32);
static void (*steamworks_Shutdown)(void);

static void
open_library(mrb_state *mrb)
{
#ifdef _WIN32
  mrb_carbuncle_steamworks_handle = LoadLibraryA(LIBRARY_NAME);
#else
  mrb_carbuncle_steamworks_handle = dlopen(LIBRARY_NAME, RTLD_LAZY);
#endif
 if (!mrb_carbuncle_steamworks_handle)
 {
   mrb_raise(mrb, E_RUNTIME_ERROR, "Failed to load steamworks API");
 }
}

static LibraryFunction
load_function(mrb_state *mrb, const char *name)
{
  LibraryFunction result;
#ifdef _WIN32
  result = GetProcAddress(mrb_carbuncle_steamworks_handle, name);
#elif __unix__
  result = dlsym(mrb_carbuncle_steamworks_handle, name);
#endif
  if (!result)
  {
    mrb_raisef(mrb, E_RUNTIME_ERROR, "Failed to load steamworks API function %s", name);
  }
}

#define LOAD(var, name) var = (decltype(var))load_function(mrb, name)

static void
load_functions(mrb_state *mrb)
{
  LOAD(steamworks_RestartAppIfNecessary, "SteamAPI_RestartAppIfNecessary");
  LOAD(steamworks_Init, "SteamAPI_Init");
  LOAD(steamworks_Shutdown, "SteamAPI_Shutdown");
}

#undef LOAD

static mrb_value
steam_load(mrb_state *mrb, mrb_value self)
{
  mrb_int app_id = 0;
  mrb_get_args(mrb, "|i", &app_id);
  /* Already loaded, skip */
  if (mrb_carbuncle_steamworks_handle)
  {
    return self;
  }
  open_library(mrb);
  load_functions(mrb);
  if (steamworks_RestartAppIfNecessary(app_id))
  {
    exit(EXIT_FAILURE);
  }
  if (!steamworks_Init())
  {
    mrb_raise(mrb, E_RUNTIME_ERROR, "Failed to initialize steamworks API");
  }
  return self;
}

static mrb_value
steam_unload(mrb_state *mrb, mrb_value self)
{
  if (mrb_carbuncle_steamworks_handle)
  {
#ifdef _WIN32
    FreeLibrary(mrb_carbuncle_steamworks_handle);
#else
    dlclose(mrb_carbuncle_steamworks_handle);
    mrb_carbuncle_steamworks_handle = NULL;
#endif
    mrb_carbuncle_steamworks_handle = NULL;
  }
  return self;
}

extern "C" void
mrb_carbuncle_steamworks_gem_init(mrb_state *mrb)
{
  struct RClass *steam = mrb_define_module(mrb, "Steamworks");

  mrb_define_module_function(mrb, steam, "load", steam_load, MRB_ARGS_OPT(1));
  mrb_define_module_function(mrb, steam, "unload", steam_unload, MRB_ARGS_NONE());
}

extern "C" void
mrb_carbuncle_steamworks_gem_final(mrb_state *mrb)
{
}
