#include "components.h"
#include <stdlib.h>
#include <string.h>
#include "magic6502.h"
#include "magic2c02.h"
#include "window.h"

unsigned char* cpu_memory_access(void* ctx, unsigned short addr) {
  unsigned final_addr = addr;
  if (addr < 0x2000) {
    /* ram mirror */
    final_addr = addr % 0x800;
  } else if (addr >= 0x2000 && addr < 0x4000) {
    /* io register mirror */
    final_addr = ((addr - 0x2000) % 8) + 0x2000;
  }
  return &((magicnes_ctx*)ctx)->memory[final_addr];
}

void interrupt_from_ppu(void* ctx) {
  magic6502_interrupt(((magicnes_ctx*)ctx)->cpu_ctx, MAGIC6502_INT_NMI);
}

magicnes_ctx* init_nes_components() {
  magicnes_ctx* ctx = (magicnes_ctx*)malloc(sizeof(magicnes_ctx));
  memset(ctx, 0, sizeof(unsigned char) * 65536);

  ctx->memory = (unsigned char*)malloc(sizeof(unsigned char*) * 65536);
  memset(ctx->memory, 0, sizeof(unsigned char) * 65536);

  ctx->cpu_ctx = magic6502_init(ctx->memory, cpu_memory_access);
  ctx->ppu_ctx = magic2c02_init(cpu_memory_access, interrupt_from_ppu, ctx, render_window);

  return ctx;
}

void free_nes_components(magicnes_ctx* ctx) {
  magic2c02_free(ctx->ppu_ctx);
  magic6502_free(ctx->cpu_ctx);
  free(ctx->memory);
  free(ctx);
}
