NAME=main
BIN=hi

OBJCOPY = objcopy
LD = ld

CXX = g++
CXXFLAGS=-std=c++11 -pedantic -Wall -O2 -mtune=arm7tdmi -mcpu=arm7tdmi -fno-exceptions -fno-rtti -fno-asynchronous-unwind-tables -nostdinc -nostdinc++ -ffreestanding

CC = gcc
CFLAGS=-std=c99 -pedantic -Wall -O2 -mtune=arm7tdmi -mcpu=arm7tdmi -fno-asynchronous-unwind-tables -nostdinc -ffreestanding

GBA = VisualBoyAdvance.exe

.PHONY: all clean run

all: $(BIN).gba


$(BIN).elf: main.o noreturn.o
	$(LD)  -N -e main main.o noreturn.o -o $(BIN).elf

$(BIN).gba: $(BIN).elf
	$(OBJCOPY) -O binary $(BIN).elf $(BIN).gba
 
clean:
	del *.gba *.o *.elf

run: $(BIN).gba
	$(GBA) $(BIN).gba

main.o: main.cpp noreturn.hpp Graphic.hpp
noreturn.o: noreturn.cpp noreturn.hpp
