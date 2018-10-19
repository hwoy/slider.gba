![](https://raw.githubusercontent.com/hwoy/slider.gba/master/res/slider.png?raw=true)

Port [Slider game](https://github.com/hwoy/slider) to GameBoy Advance on VisualBoy Advance

Download [slider.gba](https://raw.githubusercontent.com/hwoy/slider.gba/master/rom/slider.gba) for [VisualBoy Advance](https://jaist.dl.sourceforge.net/project/vba/VisualBoyAdvance/1.7.2/VisualBoyAdvance-1.7.2.zip)

# What is a slider.gba
- slider.gba is a very simple NxN slide puzzle game that can be run on GameBoy Advance. 
- It has been written in ISO C++11 abstraction.
- No special library(libheart) or even libc, except libgcc for basic software math function(div and mod).
- Work great on [VisualBoy Advance](https://jaist.dl.sourceforge.net/project/vba/VisualBoyAdvance/1.7.2/VisualBoyAdvance-1.7.2.zip), [VisualBoy Advance-m](https://github.com/visualboyadvance-m/visualboyadvance-m/releases) and [BoycottAdvance](http://www.emulator-zone.com/files/emulators/gba/ba/ba-028.zip).
 
### Keypad
- UP     == moves down block
- DOWN   == moves up block
- LEFT   == moves left block to right
- RIGHT  == moves right block to left
- START  == reset a board
- A      == pred origin random seed and init board
- B      == succ origin random seed and init board

### Game Play
- Complete block arrangement


### Tools for slider.gba

- **Toolchain:** [bleeding-edge-toolchain for ARM-eabi](http://www.freddiechopin.info/en/download/category/11-bleeding-edge-toolchain)
- **GameBoy Advance emulator:** [VisualBoy Advance](https://jaist.dl.sourceforge.net/project/vba/VisualBoyAdvance/1.7.2/VisualBoyAdvance-1.7.2.zip)
- **Builder:** [GNU make for Windows](http://ftp.gnu.org/gnu/make/?C=M;O=D) can be compiled from source by [mingw-w64](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/) (mingw-w64 bundles GNU make)

### How to make

- Setting up environment such as set up **PATH** and config **Makefile**
- Let's make it!!!!

```sh

$ make

```

- If it success , you will see [slider.gba](https://raw.githubusercontent.com/hwoy/slider.gba/master/rom/slider.gba)


### Contact me. 
- Web: https://github.com/hwoy 
- Email: zaintcorp@yahoo.com 
- Facebook: http://www.facebook.com/dead-root 