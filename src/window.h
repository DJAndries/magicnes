#ifndef WINDOW_H
#define WINDOW_H

#include "structs.h"

void init_window(magicnes_ctx* ctx);
void render_window(void* pctx, unsigned char* screen_output);
void destroy_window(magicnes_ctx* ctx);

#endif
