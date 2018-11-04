@echo off

SET TOOLCHAIN=C:\arm-none-eabi-gcc-8.2.0-180726

SET GIT=C:\Git

SET NATIVECOMPILER=C:\mingw64-toolchain

set PATH=%TOOLCHAIN%\bin;%GIT%\bin;%GIT%\usr\bin;%NATIVECOMPILER%\bin;%PATH%

cmd.exe /K title GCC-arm-eabi
