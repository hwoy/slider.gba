#include "slider.h"
#include "minstd.h"

void swap(unsigned int* const a, unsigned int* const b)
{
    unsigned int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

unsigned int getindex(const unsigned int* const sq, unsigned int blank, unsigned int hw)
{
    unsigned int i;

    for (i = 0; i < hw * hw; ++i) {
        if (sq[i] == blank)
            return i;
    }

    return -1u;
}

void getxy(unsigned int index, struct point* const p, unsigned int hw)
{
    p->y = index / hw;
    p->x = index % hw;
}

unsigned int slide(unsigned int* const sq, unsigned int kid, unsigned int _index, unsigned int hw)
{
    struct point p;
    unsigned int index, ret = -1U;

    getxy(index = getindex(sq, _index, hw), &p, hw);

    if (kid == cmd_up && p.y > 0) {
        swap(sq + index, sq + index - hw);
        ret = kid;
    }

    else if (kid == cmd_down && p.y < hw - 1) {
        swap(sq + index, sq + index + hw);
        ret = kid;
    }

    else if (kid == cmd_left && p.x > 0) {
        swap(sq + index - 1, sq + index);
        ret = kid;
    }

    else if (kid == cmd_right && p.x < hw - 1) {
        swap(sq + index + 1, sq + index);
        ret = kid;
    }

    return ret;
}

void slidesq(unsigned int* const sq, const unsigned int* const cmdsq, unsigned int n, unsigned int index, unsigned int hw)
{
    unsigned int i;
    for (i = 0; i < n; ++i)
        slide(sq, cmdsq[i], index, hw);
}

unsigned int initsq(unsigned int* const sq, unsigned int hw)
{
    unsigned int i;

    for (i = 0; i < hw * hw; ++i)
        sq[i] = i;

    return hw;
}

unsigned int canmovesq(const unsigned int* const sq, unsigned int index, unsigned int hw)
{
    struct point p;
    unsigned int i = 0xf;

    getxy(index = getindex(sq, index, hw), &p, hw);

    if (!p.y)
        i ^= (1 << 0);
    if (!p.x)
        i ^= (1 << 2);

    if (p.y == hw - 1)
        i ^= (1 << 1);
    if (p.x == hw - 1)
        i ^= (1 << 3);

    return i;
}

unsigned int extractcanmovesq(unsigned int* const d, unsigned int value)
{
    unsigned int i, j;

    for (i = 0, j = 0; i < 4; ++i) {
        if (value & (1 << i))
            d[j++] = i;
    }

    return j;
}

unsigned int randomsq(unsigned int* const sq, unsigned int index, unsigned int hw, unsigned int* seed)
{
    unsigned int i, d[4], j;

    do {
        for (i = 0; i < RANDLOOP; ++i) {
            j = extractcanmovesq(d, canmovesq(sq, index, hw));
            slide(sq, d[minstd_rand(seed) % j], index, hw);
        }
    } while (gameid(sq, hw) == gid_over);

    return RANDLOOP;
}

unsigned int gameid(const unsigned int* const sq, unsigned int hw)
{
    unsigned int i;

    for (i = 0; i < hw * hw; ++i)
        if (sq[i] != i)
            return gid_normal;

    return gid_over;
}

