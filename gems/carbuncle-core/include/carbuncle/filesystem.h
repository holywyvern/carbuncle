#ifndef CARBUNCLE_FILESYSTEM_H
#define CARBUNCLE_FILESYSTEM_H

#include <mruby.h>

#include "raylib.h"

#ifdef __cplusplus
extern "C" {
#endif

void
mrb_init_carbuncle_filesystem(mrb_state *mrb);

char *
mrb_carbuncle_load_file(mrb_state *mrb, const char *filename, size_t *size);

char *
mrb_carbuncle_load_file_text(mrb_state *mrb, const char *filename);

Image
LoadCarbuncleImage(mrb_state *mrb, const char *filename);

Texture
LoadCarbuncleTexture(mrb_state *mrb, const char *filename);

Music
LoadCarbuncleMusic(mrb_state *mrb, const char *filename);

Sound
LoadCarbuncleSound(mrb_state *mrb, const char *filename);

#ifdef __EMSCRIPTEN__

void
mrb_carbuncle_fetch_file(mrb_state *mrb, const char *filename);

#endif

#ifdef __cplusplus
}
#endif

#endif
