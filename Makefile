NAME=main
BIN=slider

OBJCOPY = arm-none-eabi-objcopy
LD = arm-none-eabi-ld

FLAGS = -pedantic -Wall -mtune=arm7tdmi -mcpu=arm7tdmi -ffreestanding -O2 -ffast-math -mlong-calls -faggressive-loop-optimizations -fno-builtin -fno-asynchronous-unwind-tables

CXX = arm-none-eabi-g++
CXXFLAGS=-std=c++11 $(FLAGS) -fno-exceptions -fno-rtti -nostdinc -nostdinc++

CC = arm-none-eabi-gcc
CFLAGS=-std=c99  $(FLAGS) -nostdinc


GBA = VisualBoyAdvance.exe

.PHONY: all clean run

all: $(BIN).gba $(BIN)-actual-GBA.gba

$(BIN)-actual-GBA.elf: loader.o main.o slider.o minstd.o lcg.o
		$(CC) -static -nostartfiles -nostdlib -Wl,-Map=$(BIN)-actual-GBA.map,-N,-Ttext,0x80000C0 loader.o main.o slider.o minstd.o lcg.o -o $(BIN)-actual-GBA.elf -static-libgcc -lgcc

$(BIN)-actual-GBA.gba: gha/gha.exe $(BIN)-actual-GBA.elf
		$(OBJCOPY) -O binary $(BIN)-actual-GBA.elf $(BIN)-actual-GBA.noheader
		gha/gha.exe $(BIN)-actual-GBA.noheader $(BIN)-actual-GBA.gba

$(BIN).elf: loader.o main.o slider.o minstd.o lcg.o
		$(CC) -static -nostartfiles -nostdlib -Wl,-Map=$(BIN).map,-N,-Ttext,0x8000000 loader.o main.o slider.o minstd.o lcg.o -o $(BIN).elf -static-libgcc -lgcc

$(BIN).gba: $(BIN).elf
	$(OBJCOPY) -O binary $(BIN).elf $(BIN).gba

gha/gha.exe:
	make -C gha
 
clean:
	rm -rf *.txt *.gba *.o *.elf *.map $(BIN)-actual-GBA.noheader
	make -C gha clean

run: $(BIN).gba
	$(GBA) $(BIN).gba




main.o: main.cpp slider.h arm7type.h Graphic.hpp Keypad.hpp Font.hpp \
 Draw.hpp Square.hpp
lcg.o: lcg.c lcg.h arm7type.h
loader.o: loader.c
minstd.o: minstd.c minstd.h arm7type.h lcg.h
slider.o: slider.c slider.h arm7type.h minstd.h lcg.h

