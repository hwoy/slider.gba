![](https://raw.githubusercontent.com/hwoy/slider.gba/master/res/slider.png?raw=true)

Port [Slider game](https://github.com/hwoy/slider) to Gameboy advance on visualboy advance

[Download](https://raw.githubusercontent.com/hwoy/slider.gba/master/rom/slider.gba) slider.gba for [visualboy advance](https://jaist.dl.sourceforge.net/project/vba/VisualBoyAdvance/1.7.2/VisualBoyAdvance-1.7.2.zip)

# What is a slider.gba
- slider.gba is a very simple 3x3 board game that run on Gameboy advance. 
- It has been written in ISO C++11 abstraction.
 
### Keypad
- UP     == moves block down
- DOWN   == moves block up
- LEFT   == moves block right
- RIGHT  == moves block left
- START  == reset a board
- SELECT == succ random seed and init board

### Game Play
- Complete block arrangement


### Tools for slider.gba

- **Toolchain:** [Mingw toolchains for ARM-eabi](http://sysprogs.com/files/gnutoolchains/arm-eabi/arm-eabi-gcc7.2.0-r3.exe)
- **Gameboy advance enulator:** [visualboy advance](https://jaist.dl.sourceforge.net/project/vba/VisualBoyAdvance/1.7.2/VisualBoyAdvance-1.7.2.zip)
- **Builder:** GNU make for Windows that comes with Native Compiler Mingw-w64

### How to make

- Setting up environment such as set **PATH** and config **Makefile**
- Let's make it!!!!

```sh

$ make

```

- If it success , you will see slider.gba


### Contact me. 
- Web: https://github.com/hwoy 
- Email: zaintcorp@yahoo.com 
- Facebook: http://www.facebook.com/dead-root 