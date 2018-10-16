NAME=main
BIN=slider

OBJCOPY = arm-eabi-objcopy
LD = arm-eabi-ld

FLAGS = -pedantic -Wall -mtune=arm7tdmi -mcpu=arm7tdmi -ffreestanding -O3 -ffast-math -mlong-calls -faggressive-loop-optimizations

CXX = arm-eabi-g++
CXXFLAGS=-std=c++11  -fno-exceptions -fno-rtti -fno-asynchronous-unwind-tables -nostdinc -nostdinc++ $(FLAGS)

CC = arm-eabi-gcc
CFLAGS=-std=c99 -fno-asynchronous-unwind-tables -nostdinc -nostdlib $(FLAGS)

LDFLAGS = -LC:\gcc-arm-eabi\lib\gcc\arm-eabi\7.2.0

GBA = VisualBoyAdvance.exe

.PHONY: all clean run

all: $(BIN).gba


$(BIN).elf: loader.o main.o slider.o minstd.o lcg.o
		$(LD) $(LDFLAGS) -Map=$(BIN).map -N -e _start -Ttext 0x8000000 loader.o main.o slider.o minstd.o lcg.o -o $(BIN).elf -lgcc

$(BIN).gba: $(BIN).elf
	$(OBJCOPY) -O binary $(BIN).elf $(BIN).gba
 
clean:
	del *.gba *.o *.elf *.map

run: $(BIN).gba
	$(GBA) $(BIN).gba



lcg.o: lcg.cpp lcg.h arm7type.hpp
loader.o: loader.cpp
main.o: main.cpp slider.h arm7type.hpp Graphic.hpp Keypad.hpp Font.hpp \
 Draw.hpp Square.hpp
minstd.o: minstd.cpp minstd.h arm7type.hpp lcg.h
slider.o: slider.cpp slider.h arm7type.hpp minstd.h lcg.h
