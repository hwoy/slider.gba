![](https://raw.githubusercontent.com/hwoy/slider.gba/master/res/slider.png?raw=true)

Port [Slider game](https://github.com/hwoy/slider) to GameBoy Advance on mGBA

Download [slider-actual-GBA.gba](https://raw.githubusercontent.com/hwoy/slider.gba/master/rom/slider-actual-GBA.gba) for [mGBA](https://mgba.io/downloads.html)

# What is a slider.gba
- slider.gba is a very simple NxN slide puzzle game that can be run on GameBoy Advance. 
- It has been written in modern ISO C++14(C++11 syntax and C++14 STL supports video memory iterator in Grange Struct) abstraction except core game logic originally written in C.
- No special library(libheart) ,except libc for memcpy(if in use), libgcc for basic software math function(div and mod).
- Work great on [mGBA](https://mgba.io/downloads.html), [higan](https://download.byuu.org/higan_v106-windows.7z), [VisualBoy Advance](https://jaist.dl.sourceforge.net/project/vba/VisualBoyAdvance/1.7.2/VisualBoyAdvance-1.7.2.zip), [VisualBoy Advance-m](https://github.com/visualboyadvance-m/visualboyadvance-m/releases) and [BoycottAdvance](http://www.emulator-zone.com/files/emulators/gba/ba/ba-028.zip).
 
### Keypad

```sh

- UP     == moves down block
- DOWN   == moves up block
- LEFT   == moves left block to right
- RIGHT  == moves right block to left
- START  == reset a board
- SELECT == Hold SELECT for complete board representation
- A      == pred origin random seed and init board
- B      == succ origin random seed and init board

```

### Game Play
- Complete block arrangement


### Tools for slider.gba

- **Toolchain:** [bleeding-edge-toolchain for ARM-eabi](http://www.freddiechopin.info/en/download/category/11-bleeding-edge-toolchain)
- **GameBoy Advance emulator:** [higan](https://mprd.se/media/emulators/files/higan_v106-windows.7z)
- **Builder:** [GNU make for Windows](http://ftp.gnu.org/gnu/make/?C=M;O=D) can be compiled from source by [mingw-w64](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/) (mingw-w64 bundles GNU make)
- **Native Compiler:** Native C compiler ([mingw-w64](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/)) for compiling [gbafix2](https://github.com/hwoy/gbafix2) (ROM GameBoy Advance Head Builder & Fixer)
- **gbafix2:** [gbafix2](https://github.com/hwoy/gbafix2) for ROM header
### How to make

- Setting up environment such as set up **PATH** and config **Makefile**
- Let's make it!!!!

```sh

$ make

```

- If it success , you will see 
	- [slider.gba](https://raw.githubusercontent.com/hwoy/slider.gba/master/rom/slider.gba) for RAW instruction codes (0x8000000 entry point slider.gba)
	- [slider-actual-GBA.gba](https://raw.githubusercontent.com/hwoy/slider.gba/master/rom/slider-actual-GBA.gba) for actual ROM (slider-actual-GBA.gba = 0xC0 bytes of header + 0x80000C0 entry point slider-actual-GBA.noheader)
- [gbafix2](https://github.com/hwoy/gbafix2) can add 0xC0 bytes of header to raw .gba file
- Some emulator such as [higan](https://mprd.se/media/emulators/files/higan_v106-windows.7z) requires actual ROM

### Special Thanks
- Awesome fonts from [https://github.com/Peter-black/SuperCrateBox-GBA](https://github.com/Peter-black/SuperCrateBox-GBA) included in Font.hpp

### Contact me. 
- Web: https://github.com/hwoy 
- Email: zaintcorp@yahoo.com 
- Facebook: http://www.facebook.com/dead-root
