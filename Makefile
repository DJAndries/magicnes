sdl_lib_dir = ../sdl
magic2c02_lib_dir = ../magic2c02
magic6502_lib_dir = ../magic6502

all: obj bin bin/libmagic6502.so bin/libmagic2c02.so bin/test_rom.nes bin/color_test.nes bin/magicnes

bin/magicnes: obj/main.o obj/components.o obj/execute.o obj/nes_file.o obj/window.o obj/controller.o
	gcc -lm -o bin/magicnes obj/main.o obj/components.o obj/execute.o obj/nes_file.o \
		obj/window.o obj/controller.o \
		-L$(magic6502_lib_dir)/lib -L$(magic2c02_lib_dir)/lib -L$(sdl_lib_dir)/lib -lmingw32 \
		-lSDL2main -lSDL2 -l:libmagic6502.so -l:libmagic2c02.so


bin/libmagic2c02.so:
	cp $(magic2c02_lib_dir)/lib/libmagic2c02.so ./bin/libmagic2c02.so

bin/libmagic6502.so:
	cp $(magic6502_lib_dir)/lib/libmagic6502.so ./bin/libmagic6502.so

bin/test_rom.nes:
	cp ./test/dk.nes ./bin/test_rom.nes

bin/color_test.nes:
	cp ./test/color_test.nes ./bin/color_test.nes

obj/%.o: src/%.c
	gcc -Wall -O -c $< -I$(magic2c02_lib_dir)/include -I$(magic6502_lib_dir)/include \
	 	-I$(sdl_lib_dir)/include -o $@

obj:
	mkdir obj

bin:
	mkdir bin

clean:
	rm -f obj/* bin/*
