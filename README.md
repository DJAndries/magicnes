# magicnes
#### Partially working NES emulator (no sound)

[*Download Windows Binaries Here*](https://github.com/DJAndries/magicnes/releases)

Kind of working, kind of broken

Not supported:
- Everything not supported in magic2c02:
  - Sprite flipping
  - Nametable mirroring (except for horizontal mirroring)
  - PPU master/slave select
  - Color emphasis/PPUMASK register
  - PAL (NTSC only)
- Sound
- Any game with a memory mapper that isn't 000

Donkey Kong kinda works :)

## Building

Depends on SDL 2. The Makefile assumes you have an SDL build in the parent directory.

Also depends on my `magic2c02` and `magic6502` libraries. Clone those in the parent directory and build them.

Should build with gcc/make.
I used MinGW (32-bit) and MSYS to build this on Windows.

```
make
```

## Execution

Color test and DK ROMs are included. Run `magicnes` in the bin folder after build.

## Controls

- A is A
- B is B
- Right shift is select
- Return is start
- Up is up
- Down is down
- Left is left
- Right is right
- Escape is exit

Controls can be changed using the `key_mappings` array in `controller.c`.
