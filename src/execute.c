#include "execute.h"
#include "controller.h"
#include <string.h>
#include "magic6502.h"
#include "magic2c02.h"

#define PPU_FRAME_CPU_CYCLES 113
#define CPU_CLOCK_RATE 1789773
#define PERIOD_MS 50.0

void period_delay(magicnes_ctx* ctx) {
  if (ctx->period_start_ticks && ctx->period_cycle_count >= ctx->cycles_in_period) {
    ctx->period_cycle_count = 0;
    ctx->period_end_ticks = ctx->period_start_ticks + PERIOD_MS;
    ctx->period_start_ticks = 0;
  } else if (ctx->period_end_ticks) {
    if (SDL_GetTicks() >= ctx->period_end_ticks) {
      ctx->period_end_ticks = 0;
      ctx->period_start_ticks = SDL_GetTicks();
    }
  }
}

void execute(magicnes_ctx* ctx) {
  unsigned char cpu_cycle_count;

  #ifdef LOGGING_ENABLED
    printf("pc: %x inst: %x x: %x y: %x sp: %x %x %x %x\n", ctx->cpu_ctx->pc,
      ctx->memory[ctx->cpu_ctx->pc], ctx->cpu_ctx->x, ctx->cpu_ctx->y,
      ctx->cpu_ctx->sp, ctx->memory[0x100 | (ctx->cpu_ctx->sp + 1)],
      ctx->memory[0x100 | (ctx->cpu_ctx->sp + 2)],
      ctx->memory[0x100 | (ctx->cpu_ctx->sp + 3)]);
  #endif

  cpu_cycle_count = magic6502_exec(ctx->cpu_ctx);
  ctx->period_cycle_count += cpu_cycle_count;
  ctx->frame_cpu_cycle_count += cpu_cycle_count;

  process_controller_registers(ctx);
  magic2c02_process_registers(ctx->ppu_ctx, ctx->cpu_ctx->address_accessed);

  #ifdef LOGGING_ENABLED
    if (ctx->cpu_ctx->address_accessed >= 0x2000 && ctx->cpu_ctx->address_accessed <= 0x2007) {
      printf("PPU LATCH %x\n", ctx->cpu_ctx->address_accessed);
    }
  #endif

  if (ctx->frame_cpu_cycle_count >= PPU_FRAME_CPU_CYCLES) {
    ctx->frame_cpu_cycle_count = 0;
    magic2c02_render_scanline(ctx->ppu_ctx);
  }
}

void execution_loop(magicnes_ctx* ctx) {
  magic6502_interrupt(ctx->cpu_ctx, MAGIC6502_INT_RESET);

  ctx->cycles_in_period = (CPU_CLOCK_RATE / 1000.0) * PERIOD_MS;
  ctx->period_start_ticks = SDL_GetTicks();

  while (1) {
    if (process_keyboard_events(ctx)) {
      /* Quit emulator */
      return;
    }
    if (ctx->period_start_ticks) {
      execute(ctx);
    } else {
      /* sleep cpu if not in execution period */
      SDL_Delay(1);
    }

    period_delay(ctx);
  }
}
