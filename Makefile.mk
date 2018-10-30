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

OBJECTS = main-$(BIN).o lcg-$(BIN).o loader-$(BIN).o minstd-$(BIN).o slider-$(BIN).o

INSTALLDIR = rom
.PHONY: all clean run install

all: $(BIN).gba $(BIN)-actual-GBA.gba

$(BIN)-actual-GBA.elf: $(OBJECTS)
		$(CC) -static -nostartfiles -nostdlib -Wl,-Map=$(BIN)-actual-GBA.map,-N,-Ttext,0x80000C0 $^ -o $@ -static-libgcc -lgcc -lc

$(BIN)-actual-GBA.gba: gbafix2/gbafix2.exe $(BIN)-actual-GBA.elf
		$(OBJCOPY) -O binary $(BIN)-actual-GBA.elf $(BIN)-actual-GBA.noheader
		gbafix2/gbafix2.exe $(BIN)-actual-GBA.noheader -o:$@ -a -t:Slider -r:1 -c:Hwoy -p

$(BIN).elf: $(OBJECTS)
		$(CC) -static -nostartfiles -nostdlib -Wl,-Map=$(BIN).map,-N,-Ttext,0x8000000 $^ -o $@ -static-libgcc -lgcc -lc

$(BIN).gba: $(BIN).elf
	$(OBJCOPY) -O binary $(BIN).elf $@

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


%-$(BIN).o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

%-$(BIN).o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<


