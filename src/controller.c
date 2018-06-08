#include "controller.h"
#include "SDL2/SDL.h"

/* Controller key mappings*/
SDL_Keycode key_mappings[8] = {
  SDLK_a, SDLK_b, /* Select */ SDLK_RSHIFT, /* Start */ SDLK_RETURN, SDLK_UP, SDLK_DOWN,
  SDLK_LEFT, SDLK_RIGHT
};

void input_write(magicnes_ctx* ctx) {
  if (ctx->memory[0x4016] & 0x01) {
    ctx->controller_status.is_strobe_high = 1;
    ctx->controller_status.next_bit_index = 0;
    ctx->memory[0x4016] = ctx->controller_status.button_status[ctx->controller_status.next_bit_index++];
  } else {
    ctx->controller_status.is_strobe_high = 0;
  }
}

void output_read(magicnes_ctx* ctx) {
  ctx->memory[0x4016] = ctx->controller_status.button_status[ctx->controller_status.next_bit_index];

  ctx->controller_status.next_bit_index = (ctx->controller_status.next_bit_index + 1) % 8;

  if (ctx->controller_status.is_strobe_high) {
    ctx->controller_status.next_bit_index = 0;
  }
}

void process_controller_registers(magicnes_ctx* ctx) {
  if (ctx->cpu_ctx->address_accessed != 0x4016) {
    return;
  }
  if (ctx->cpu_ctx->is_last_instr_write) {
    input_write(ctx);
  } else {
    output_read(ctx);
  }
}

int process_keyboard_events(magicnes_ctx* ctx) {
  SDL_Event event;
  unsigned char keydown;
  unsigned char i;

  while (SDL_PollEvent(&event)) {
    if (event.type != SDL_KEYDOWN && event.type != SDL_KEYUP) {
      continue;
    }
    keydown = event.type == SDL_KEYDOWN;

    /* Quit emulator */
    if (event.key.keysym.sym == SDLK_ESCAPE) {
      return 1;
    }

    /* Set controller output bit for key */
    for (i = 0; i < 8; i++) {
      if (event.key.keysym.sym == key_mappings[i]) {
        ctx->controller_status.button_status[i] = keydown;
        break;
      }
    }
  }
  return 0;
}
