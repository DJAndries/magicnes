#include "execute.h"
#include <string.h>
#include "SDL2/SDL.h"
#include "magic6502.h"
#include "magic2c02.h"

#define PPU_FRAME_CPU_CYCLES 113
#define CPU_CLOCK_RATE 1789773
#define PERIOD_MS 100.0

char check_escape(SDL_Event* event) {
  if (event->type == SDL_KEYUP && event->key.keysym.sym == SDLK_ESCAPE) {
    return 1;
  }
  return 0;
}

void period_delay(magicnes_ctx* ctx) {
  if (ctx->period_start_ticks && ctx->period_cycle_count >= ctx->cycles_in_period) {
    ctx->period_cycle_count = 0;
    ctx->period_end_ticks = ctx->period_start_ticks + 100;
    ctx->period_start_ticks = 0;
  } else if (ctx->period_end_ticks) {
    if (SDL_GetTicks() >= ctx->period_end_ticks) {
      ctx->period_end_ticks = 0;
      ctx->period_start_ticks = SDL_GetTicks();
    }
  }
}

void execute(magicnes_ctx* ctx) {
  unsigned char cpu_cycle_count = magic6502_exec(ctx->cpu_ctx);
  ctx->period_cycle_count += cpu_cycle_count;
  ctx->frame_cpu_cycle_count += cpu_cycle_count;

  magic2c02_process_registers(ctx->ppu_ctx, ctx->cpu_ctx->address_accessed);

  if (ctx->frame_cpu_cycle_count >= PPU_FRAME_CPU_CYCLES) {
    ctx->frame_cpu_cycle_count = 0;
    magic2c02_render_scanline(ctx->ppu_ctx);
  }
}

void execution_loop(magicnes_ctx* ctx) {
  SDL_Event event;

  ctx->cycles_in_period = (CPU_CLOCK_RATE / 1000.0) * PERIOD_MS;
  ctx->period_start_ticks = SDL_GetTicks();

  while (1) {
    while (SDL_PollEvent(&event)) {
      if (check_escape(&event)) {
        break;
      }
    }
    if (ctx->period_start_ticks) {
      execute(ctx);
    }
    period_delay(ctx);
  }
}
