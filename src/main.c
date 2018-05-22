#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h"

#include "structs.h"
#include "components.h"
#include "window.h"
#include "execute.h"
#include "nes_file.h"

int main(int argc, char** argv) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    printf("SDL init failed: %s\n", SDL_GetError());
    return 1;
  }

  magicnes_ctx* ctx = init_nes_components();

  if (!load_rom(ctx, "test_rom.nes")) {
    printf("Cannot open rom file");
    free_nes_components(ctx);
    return 1;
  }

  init_window(ctx);

  execution_loop(ctx);

  destroy_window(ctx);
  free_nes_components(ctx);

  return 0;
}
