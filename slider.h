#ifndef _SLIDER_H_
#define _SLIDER_H_

#ifndef RANDLOOP
#define RANDLOOP 1024
#endif

#include "arm7type.hpp"

struct point {
    u32arm_t y, x;
};

enum {
    cmd_up,
    cmd_down,
    cmd_left,
    cmd_right
};

enum {
    gid_normal,
    gid_over
};

void swap(u32arm_t* const a, u32arm_t* const b);

u32arm_t getindex(const u32arm_t* const sq, u32arm_t blank, u32arm_t hw);

void getxy(u32arm_t index, struct point* const p, u32arm_t hw);

u32arm_t slide(u32arm_t* const sq, u32arm_t kid, u32arm_t _index, u32arm_t hw);

void slidesq(u32arm_t* const sq, const u32arm_t* const cmdsq, u32arm_t n, u32arm_t index, u32arm_t hw);

u32arm_t initsq(u32arm_t* const sq, u32arm_t hw);

u32arm_t canmovesq(const u32arm_t* const sq, u32arm_t index, u32arm_t hw);

u32arm_t extractcanmovesq(u32arm_t* const d, u32arm_t value);

u32arm_t randomsq(u32arm_t* const sq, u32arm_t index, u32arm_t hw, u32arm_t* seed);

u32arm_t gameid(const u32arm_t* const sq, u32arm_t hw);


#endif