#ifndef STRUCTS_H
#define STRUCTS_H

#include "magic2c02.h"
#include "magic6502.h"
#include "SDL2/SDL.h"

typedef struct {
  unsigned char button_status[8];
  unsigned char next_bit_index;
  unsigned char is_strobe_high;
} magicnes_controller_state;

typedef struct {
  magic6502_ctx* cpu_ctx;
  magic2c02_ctx* ppu_ctx;

  SDL_Window* win;
  SDL_Renderer* renderer;
  SDL_Texture* buffer;

  unsigned char* memory;

  unsigned short frame_cpu_cycle_count;

  magicnes_controller_state controller_status;

  unsigned int period_start_ticks;
  unsigned int period_end_ticks;
  unsigned long period_cycle_count;
  double cycles_in_period;
} magicnes_ctx;

#endif
