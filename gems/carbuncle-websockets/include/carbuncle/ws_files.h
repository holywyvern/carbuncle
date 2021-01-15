#ifndef CARBUNCLE_WS_FILES_H
#define CARBUNCLE_WS_FILES_H

#ifdef __cplusplus
extern "C" {
#endif

static int
close_ws_file(lws_fop_fd_t *fd)
{
  int result;
  PHYSFS_File *fp;
  fp = (*fd)->filesystem_priv;
  result = PHYSFS_close(fp);
  if (result < 0) { return 1; }
  free(*fd);
  return 0;
}

static lws_fop_fd_t
open_ws_file(const struct lws_plat_file_ops *fops, const char *filename, const char *vpath, lws_fop_flags_t *flags)
{
  PHYSFS_File *fp;
  lws_fop_fd_t result;
  fp = NULL;
  
  if (flags && (*flags & LWS_O_WRONLY))
  {
    if (*flags & LWS_O_TRUNC) { fp = PHYSFS_openWrite(filename); }
    else { fp = PHYSFS_openAppend(filename); }
  }
  else { fp = PHYSFS_openRead(filename); }
  if (!fp) return NULL;
  result = malloc(sizeof *result);
  if (!result)
  {
    PHYSFS_close(fp);
    return NULL;
  }
  result->filesystem_priv = fp;
  result->len = PHYSFS_fileLength(fp);
  result->pos = PHYSFS_tell(fp);
  result->fd = 0;
  result->fops = fops;
  result->mod_time = PHYSFS_getLastModTime(filename);
  if (flags) { result->flags = *flags; }
  return result;
}

static int
read_ws_file(lws_fop_fd_t fop_fd, lws_filepos_t *amount, uint8_t *buf, lws_filepos_t len)
{
  int result;
  PHYSFS_File *fp;
  fp = fop_fd->filesystem_priv;
  result = PHYSFS_readBytes(fp, buf, len);
  if (result < 0) { return 1; }
  *amount += result;
  return 0;
}

static lws_fileofs_t
seek_cur_ws_file(lws_fop_fd_t fop_fd, lws_fileofs_t offset_from_cur_pos)
{
  lws_fileofs_t new_pos;
  PHYSFS_File *fp;
  fp = fop_fd->filesystem_priv;
  new_pos = PHYSFS_tell(fp) + offset_from_cur_pos;
  PHYSFS_seek(fp, new_pos);
  return new_pos;
}

static int
write_ws_file(lws_fop_fd_t fop_fd, lws_filepos_t *amount, uint8_t *buf, lws_filepos_t len)
{
  int result;
  PHYSFS_File *fp;
  fp = fop_fd->filesystem_priv;
  result = PHYSFS_writeBytes(fp, buf, len);
  if (result < 0) { return 1; }
  *amount = result;
  return 0;
}

#ifdef __cplusplus
}
#endif

#endif
