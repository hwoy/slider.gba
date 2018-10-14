NAME=main
BIN=hi

OBJCOPY = objcopy
LD = ld

FLAGS = -pedantic -Wall -O2 -mtune=arm7tdmi -mcpu=arm7tdmi -ffreestanding -ffast-math

CXX = g++
CXXFLAGS=-std=c++11  -fno-exceptions -fno-rtti -fno-asynchronous-unwind-tables -nostdinc -nostdinc++ $(FLAGS)

CC = gcc
CFLAGS=-std=c99 -fno-asynchronous-unwind-tables -nostdinc $(FLAGS)

GBA = VisualBoyAdvance.exe

.PHONY: all clean run

all: $(BIN).gba


$(BIN).elf: loader.o main.o
	$(LD)  -Map=$(BIN).map -N -e start -Ttext 0x8000000 loader.o main.o -o $(BIN).elf

$(BIN).gba: $(BIN).elf
	$(OBJCOPY) -O binary $(BIN).elf $(BIN).gba
 
clean:
	del *.gba *.o *.elf *.map

run: $(BIN).gba
	$(GBA) $(BIN).gba

loader.o: loader.cpp
main.o: main.cpp arm7type.hpp Graphic.hpp Keypad.hpp
