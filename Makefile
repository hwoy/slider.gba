NAME=main
BIN=slider

OBJCOPY = arm-eabi-objcopy
LD = arm-eabi-ld

FLAGS = -pedantic -Wall -mtune=arm7tdmi -mcpu=arm7tdmi -ffreestanding -O2 -ffast-math -mlong-calls -faggressive-loop-optimizations -fno-builtin -fno-asynchronous-unwind-tables

CXX = arm-eabi-g++
CXXFLAGS=-std=c++11 $(FLAGS) -fno-exceptions -fno-rtti -fno-asynchronous-unwind-tables -nostdinc -nostdinc++

CC = arm-eabi-gcc
CFLAGS=-std=c99  $(FLAGS) -nostdinc


GBA = VisualBoyAdvance.exe

.PHONY: all clean run

all: $(BIN).gba


$(BIN).elf: loader.o main.o slider.o minstd.o lcg.o
		$(CC) -nostartfiles -nostdlib -Wl,-Map=$(BIN).map,-N,-Ttext,0x8000000 loader.o main.o slider.o minstd.o lcg.o -o $(BIN).elf -lgcc

$(BIN).gba: $(BIN).elf
	$(OBJCOPY) -O binary $(BIN).elf $(BIN).gba
 
clean:
	del *.gba *.o *.elf *.map

run: $(BIN).gba
	$(GBA) $(BIN).gba




main.o: main.cpp slider.h arm7type.h Graphic.hpp Keypad.hpp Font.hpp \
 Draw.hpp Square.hpp
lcg.o: lcg.c lcg.h arm7type.h
loader.o: loader.c
minstd.o: minstd.c minstd.h arm7type.h lcg.h
slider.o: slider.c slider.h arm7type.h minstd.h lcg.h
