#include "window.h"
#include <string.h>
#include "SDL2/SDL.h"

void init_window(magicnes_ctx* ctx) {
  ctx->win = SDL_CreateWindow("magicnes", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    256, 240, 0);
  ctx->renderer = SDL_CreateRenderer(ctx->win, -1, SDL_RENDERER_ACCELERATED
    | SDL_RENDERER_TARGETTEXTURE);

  ctx->buffer = SDL_CreateTexture(ctx->renderer, SDL_PIXELFORMAT_RGB24,
    SDL_TEXTUREACCESS_STREAMING, 256, 240);
}

void render_window(void* pctx, unsigned char* screen_output) {
  int pitch = 256 * 3;
  unsigned char* pixels;
  magicnes_ctx* ctx = (magicnes_ctx*)pctx;
  SDL_LockTexture(ctx->buffer, NULL, (void**)&pixels, &pitch);
  memcpy(pixels, screen_output, 256 * 240 * 3);
  SDL_UnlockTexture(ctx->buffer);

  SDL_RenderClear(ctx->renderer);
  SDL_RenderCopy(ctx->renderer, ctx->buffer, NULL, NULL);
  SDL_RenderPresent(ctx->renderer);
}

void destroy_window(magicnes_ctx* ctx) {
  SDL_DestroyTexture(ctx->buffer);
  SDL_DestroyRenderer(ctx->renderer);
  SDL_DestroyWindow(ctx->win);
}
