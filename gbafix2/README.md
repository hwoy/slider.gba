# gbafix2
1. **gbafix2** is a ROM GameBoy Advance Head Builder & Fixer.
2. created for alternative gbafix.
3. fork from [gbafix](https://github.com/devkitPro/gba-tools/blob/master/src/gbafix.c)

### What's the differnce
1. Support concat header and rawcode, benefit for some toolchains
2. Outout file, prevents file damage
3. Support replace good header
4. Written in ansi c
5. Approve original code

### Command

```sh

gbafix2 is a ROM GameBoy Advance Head Builder & Fixer

USAGE:: gbafix2 [option] infile

-g	 => Replaced by good header
-a	 => Add header to an output file
-p	 => Pad to next exact power of 2. No minimum size
-t:	 => Patch title. Fill zero if none given
-c:	 => Patch game code (four characters)
-m:	 => Patch maker code (two characters)
-r:	 => Patch game version (number)
-o:	 => Output file (must be assigned)
-d:	 => Degug enabled (0 or 1)


```


### Contact me. 
- Web: https://github.com/hwoy 
- Email: zaintcorp@yahoo.com 
- Facebook: http://www.facebook.com/dead-root 
