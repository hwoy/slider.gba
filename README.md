![](https://raw.githubusercontent.com/hwoy/slider.gba/master/res/slider.gif?raw=true)

Port [Slider game](https://github.com/hwoy/slider) to GameBoy Advance on mGBA

Download [slider-8-actual-GBA.gba](https://raw.githubusercontent.com/hwoy/slider.gba/master/rom/slider-8-actual-GBA.gba) for [mGBA](https://mgba.io/downloads.html)

# What is a slider.gba
- slider.gba is a very simple NxN sliding puzzle game that can be run on GameBoy Advance. 
- It has been written in optimized modern ISO C++17 except core game logic originally written in C.
- No special library(libheart) ,except libc for memcpy function(if in use), libgcc for basic software math function(div and mod).
- Work great on [mGBA](https://mgba.io/downloads.html), [higan](https://download.byuu.org/higan_v106-windows.7z), [VisualBoy Advance](https://jaist.dl.sourceforge.net/project/vba/VisualBoyAdvance/1.7.2/VisualBoyAdvance-1.7.2.zip), [VisualBoy Advance-m](https://github.com/visualboyadvance-m/visualboyadvance-m/releases) and [BoycottAdvance](http://www.emulator-zone.com/files/emulators/gba/ba/ba-028.zip).

### Features
- Modern C++ style library in [Graphic.hpp](https://github.com/hwoy/slider.gba/blob/master/Graphic.hpp) and [Keypad.hpp](https://github.com/hwoy/slider.gba/blob/master/Keypad.hpp)
- Provides Iterator in Grange struct
- Higher level abstraction than C
- [Graphic.hpp](https://github.com/hwoy/slider.gba/blob/master/Graphic.hpp) supports Graphic mode 3,4,5
- [Keypad.hpp](https://github.com/hwoy/slider.gba/blob/master/Keypad.hpp) supports dual style keypad action method message() and dispatch()

```C++

#include "Graphic.hpp"
#include "Keypad.hpp"

static const char A[] = {
        0, 0, 0, 1, 1, 0, 0, 0,
        0, 0, 0, 1, 1, 0, 0, 0,
        0, 0, 1, 1, 1, 1, 0, 0,
        0, 0, 1, 1, 0, 1, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 0,
        0, 1, 1, 0, 0, 0, 1, 0,
        1, 1, 1, 0, 0, 0, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0
    };

static const char B[]={
        0, 1, 1, 1, 1, 1, 1, 0,
        0, 1, 1, 0, 0, 0, 1, 1,
        0, 1, 1, 0, 0, 0, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 0,
        0, 1, 1, 0, 0, 0, 1, 1,
        0, 1, 1, 0, 0, 0, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0
    };

int main()
{
	GraphicDevice::refdispcnt() = 0x400 | Color5::mode;

	Graphicx<Color5p1> g1;
	Graphicx<Color5p2> g2;

	g1.bgcolor(RGB15(0,31,30));
	g2.bgcolor(RGB15(20,31,40));

	{
		unsigned int i=0;
		for(auto &r:g1.grange(50,50,50+8,50+8))
			r=A[i++] ? RGB15(0,0,31):RGB15(0,31,30);
	}

	{
		unsigned int i=0;
		for(auto &r:g2.grange(50,50,50+8,50+8))
			r=B[i++] ? RGB15(31,0,0):RGB15(20,31,40);
	}
	
	Keypad<KeypadDevice> keypad;

	while(true)
	{
		const auto e = keypad.message().first;

		if(e==e.EVENT_DOWN)
		{
			g1.waitVSync();
			g1.flip();
		}
	}

	return 0;
}

```
 
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

- **Toolchain:** [MSYS2](https://www.msys2.org)
```sh
pacman -S mingw-w64-x86_64-arm-none-eabi-toolchain
```
or
```sh
pacman -S mingw-w64-clang-x86_64-arm-none-eabi-toolchain
```
- **GameBoy Advance emulator:** [mGBA](https://mgba.io/downloads.html)
- **Native Compiler:** [MSYS2](https://www.msys2.org) for build [gbafix2](https://github.com/hwoy/gbafix2) (ROM GameBoy Advance Head Builder & Fixer)
```sh
pacman -S mingw-w64-x86_64-toolchain
```
- **gbafix2:** [gbafix2](https://github.com/hwoy/gbafix2) for ROM header (included)


### How to make

- Open MSYS2
- Then let's make it!!!!

```sh
git clone --recurse-submodules https://github.com/hwoy/slider.gba.git
cd slider.gba
sh Install.sh

```

- If it success , you will see 
- [slider-8-actual-GBA.gba](https://raw.githubusercontent.com/hwoy/slider.gba/master/rom/slider-8-actual-GBA.gba) for actual ROM (slider-actual-GBA.gba = 0xC0 bytes of header + 0x80000C0 entry point slider-actual-GBA.noheader)
- [gbafix2](https://github.com/hwoy/gbafix2) can add 0xC0 bytes of header to raw .gba file

### Special Thanks
- Awesome fonts from [https://github.com/Peter-black/SuperCrateBox-GBA](https://github.com/Peter-black/SuperCrateBox-GBA) included in [Font.hpp](https://github.com/hwoy/slider.gba/blob/master/Font.hpp)

### Contact me
- Web: https://github.com/hwoy 
- Email: zaintcorp@yahoo.com 
- Facebook: http://www.facebook.com/dead-root
