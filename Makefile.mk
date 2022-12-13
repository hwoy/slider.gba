
SQLIST8 = \"123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+-0\"
SQLIST7 = \"123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmn\"
SQLIST6 = \"123456789ABCDEFGHIJKLMNOPQRSTUVWXYZa\"
SQLIST5 = \"123456789ABCDEFGHIJKLMNOP\"
SQLIST4 = \"123456789ABCDEFG\"
SQLIST3 = \"123456789\"
SQLIST2 = \"1234\"

OPTIMIZE = -Os
FLAGS = $(OPTIMIZE) -DSQLIST=$(SQLIST) -pedantic -Wall -Wextra -mtune=arm7tdmi -mcpu=arm7tdmi -ffreestanding -ffast-math -mlong-calls -fno-builtin -fno-asynchronous-unwind-tables

CXX = arm-none-eabi-g++
CXXSTD = c++17
CXXFLAGS = -std=$(CXXSTD) $(FLAGS) -fno-exceptions -fno-rtti

CC = arm-none-eabi-gcc
CSTD = c90
CFLAGS = -std=$(CSTD) $(FLAGS)

OBJCOPY = arm-none-eabi-objcopy
LD = arm-none-eabi-ld


GBA = VisualBoyAdvance.exe

OBJECTS = main-$(BIN).o lcg-$(BIN).o loader-$(BIN).o minstd-$(BIN).o slider-$(BIN).o

INSTALLDIR = rom
.PHONY: all clean run install

all: $(BIN)-actual-GBA.gba

$(BIN)-actual-GBA.elf: $(OBJECTS)
		$(CC) -static -nostartfiles -nostdlib -Wl,-s,-Map=$(BIN)-actual-GBA.map,-N,-Ttext,0x80000C0 $^ -o $@ -static-libgcc -lgcc -lc

$(BIN)-actual-GBA.gba: gbafix2/gbafix2.exe $(BIN)-actual-GBA.elf
		$(OBJCOPY) -S -O binary $(BIN)-actual-GBA.elf $(BIN)-actual-GBA.noheader
		gbafix2/gbafix2.exe $(BIN)-actual-GBA.noheader -o:$@ -a -t:Slider -r:1 -c:Hwoy -p


gbafix2/gbafix2.exe:
	make -C gbafix2
 
clean:
	rm -rf *.txt $(OBJECTS) $(BIN)-actual-GBA.gba $(BIN)-actual-GBA.elf $(BIN)-actual-GBA.map $(BIN)-actual-GBA.noheader
	make -C gbafix2 clean

run: $(BIN).gba
	$(GBA) $(BIN).gba

install: all
	cp $(BIN)-actual-GBA.gba rom
	cp $(BIN).gba rom


%-$(BIN).o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

%-$(BIN).o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<


