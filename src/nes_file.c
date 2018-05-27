#include "nes_file.h"
#include <stdio.h>

char load_rom(magicnes_ctx* ctx, char* filename) {
  FILE* fp = fopen(filename, "rb");
  if (fp == 0) {
    return 0;
  }
  fseek(fp, 0x10, SEEK_SET);
  fread(ctx->memory + 0x8000, sizeof(unsigned char), 0x8000, fp);
  fread(ctx->ppu_ctx->vm, sizeof(unsigned char), 0x2000, fp);
  fclose(fp);
  return 1;
}
