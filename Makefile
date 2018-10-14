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


$(BIN).elf: main.o noreturn.o
	$(LD)  -N -e main -Ttext 0x8000000 main.o noreturn.o -o $(BIN).elf

$(BIN).gba: $(BIN).elf
	$(OBJCOPY) -O binary $(BIN).elf $(BIN).gba
 
clean:
	del *.gba *.o *.elf

run: $(BIN).gba
	$(GBA) $(BIN).gba

main.o: main.cpp noreturn.hpp Graphic.hpp arm7type.hpp
noreturn.o: noreturn.cpp noreturn.hpp
