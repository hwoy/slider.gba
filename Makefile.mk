OBJCOPY = arm-none-eabi-objcopy
LD = arm-none-eabi-ld

SQLIST8 = \"123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+-0\"
SQLIST7 = \"123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmn\"
SQLIST6 = \"123456789ABCDEFGHIJKLMNOPQRSTUVWXYZa\"
SQLIST5 = \"123456789ABCDEFGHIJKLMNOP\"
SQLIST4 = \"123456789ABCDEFG\"
SQLIST3 = \"123456789\"
SQLIST2 = \"1234\"

FLAGS = -DSQLIST=$(SQLIST) -pedantic -Wall -Wextra -mtune=arm7tdmi -mcpu=arm7tdmi -ffreestanding -O2 -ffast-math -mlong-calls -faggressive-loop-optimizations -fno-builtin -fno-asynchronous-unwind-tables

CXX = arm-none-eabi-g++
CXXFLAGS=-std=c++14 $(FLAGS) -fno-exceptions -fno-rtti

CC = arm-none-eabi-gcc
CFLAGS=-std=c90  $(FLAGS)


GBA = VisualBoyAdvance.exe

INSTALLDIR = rom
.PHONY: all clean run install

all: $(BIN).gba $(BIN)-actual-GBA.gba

$(BIN)-actual-GBA.elf: main-$(BIN).o lcg-$(BIN).o loader-$(BIN).o minstd-$(BIN).o slider-$(BIN).o
		$(CC) -static -nostartfiles -nostdlib -Wl,-Map=$(BIN)-actual-GBA.map,-N,-Ttext,0x80000C0 main-$(BIN).o lcg-$(BIN).o loader-$(BIN).o minstd-$(BIN).o slider-$(BIN).o -o $(BIN)-actual-GBA.elf -static-libgcc -lgcc -lc

$(BIN)-actual-GBA.gba: gbafix2/gbafix2.exe $(BIN)-actual-GBA.elf
		$(OBJCOPY) -O binary $(BIN)-actual-GBA.elf $(BIN)-actual-GBA.noheader
		gbafix2/gbafix2.exe $(BIN)-actual-GBA.noheader -o:$(BIN)-actual-GBA.gba -a -t:Slider -r:1 -c:Hwoy -p

$(BIN).elf: main-$(BIN).o lcg-$(BIN).o loader-$(BIN).o minstd-$(BIN).o slider-$(BIN).o
		$(CC) -static -nostartfiles -nostdlib -Wl,-Map=$(BIN).map,-N,-Ttext,0x8000000 main-$(BIN).o lcg-$(BIN).o loader-$(BIN).o minstd-$(BIN).o slider-$(BIN).o -o $(BIN).elf -static-libgcc -lgcc -lc

$(BIN).gba: $(BIN).elf
	$(OBJCOPY) -O binary $(BIN).elf $(BIN).gba

gbafix2/gbafix2.exe:
	make -C gbafix2
 
clean:
	rm -rf *.txt main-$(BIN).o lcg-$(BIN).o loader-$(BIN).o minstd-$(BIN).o slider-$(BIN).o $(BIN)-actual-GBA.gba  $(BIN).gba $(BIN)-actual-GBA.elf $(BIN).elf $(BIN)-actual-GBA.map $(BIN).map $(BIN)-actual-GBA.noheader
	make -C gbafix2 clean

run: $(BIN).gba
	$(GBA) $(BIN).gba

install: all
	cp $(BIN)-actual-GBA.gba rom
	cp $(BIN).gba rom




main-$(BIN).o: main.cpp slider.h arm7type.h Graphic.hpp Keypad.hpp Font.hpp \
 Draw.hpp Square.hpp
	$(CXX) -c $(CXXFLAGS) -o main-$(BIN).o main.cpp

lcg-$(BIN).o: lcg.c lcg.h arm7type.h
	$(CC) -c $(CFLAGS) -o lcg-$(BIN).o lcg.c

loader-$(BIN).o: loader.c
	$(CC) -c $(CFLAGS) -o loader-$(BIN).o loader.c

minstd-$(BIN).o: minstd.c minstd.h arm7type.h lcg.h
	$(CC) -c $(CFLAGS) -o minstd-$(BIN).o minstd.c

slider-$(BIN).o: slider.c slider.h arm7type.h minstd.h lcg.h
	$(CC) -c $(CFLAGS) -o slider-$(BIN).o slider.c

