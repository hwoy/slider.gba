#ifndef _SLIDER_H_
#define _SLIDER_H_

#ifndef RANDLOOP
#define RANDLOOP 1024
#endif

struct point {
    unsigned int y, x;
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

void swap(unsigned int* const a, unsigned int* const b);

unsigned int getindex(const unsigned int* const sq, unsigned int blank, unsigned int hw);

void getxy(unsigned int index, struct point* const p, unsigned int hw);

unsigned int slide(unsigned int* const sq, unsigned int kid, unsigned int _index, unsigned int hw);

void slidesq(unsigned int* const sq, const unsigned int* const cmdsq, unsigned int n, unsigned int index, unsigned int hw);

unsigned int initsq(unsigned int* const sq, unsigned int hw);

unsigned int canmovesq(const unsigned int* const sq, unsigned int index, unsigned int hw);

unsigned int extractcanmovesq(unsigned int* const d, unsigned int value);

unsigned int randomsq(unsigned int* const sq, unsigned int index, unsigned int hw, unsigned int* seed);

unsigned int gameid(const unsigned int* const sq, unsigned int hw);


#endif