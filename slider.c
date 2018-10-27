#include "slider.h"
#include "minstd.h"

#include "arm7type.h"

void swap(u32arm_t* const a, u32arm_t* const b)
{
    u32arm_t tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

u32arm_t getindex(const u32arm_t* const sq, u32arm_t blank, u32arm_t hw)
{
    u32arm_t i;

    for (i = 0; i < hw * hw; ++i) {
        if (sq[i] == blank)
            return i;
    }

    return -1u;
}

void getxy(u32arm_t index, struct point* const p, u32arm_t hw)
{
    p->y = index / hw;
    p->x = index % hw;
}

u32arm_t slide(u32arm_t* const sq, u32arm_t kid, u32arm_t _index, u32arm_t hw)
{
    struct point p;
    u32arm_t index, ret = -1U;

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

void slidesq(u32arm_t* const sq, const u32arm_t* const cmdsq, u32arm_t n, u32arm_t index, u32arm_t hw)
{
    u32arm_t i;
    for (i = 0; i < n; ++i)
        slide(sq, cmdsq[i], index, hw);
}

u32arm_t initsq(u32arm_t* const sq, u32arm_t hw)
{
    u32arm_t i;

    for (i = 0; i < hw * hw; ++i)
        sq[i] = i;

    return hw;
}

u32arm_t canmovesq(const u32arm_t* const sq, u32arm_t index, u32arm_t hw)
{
    struct point p;
    u32arm_t i = 0xf;

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

u32arm_t extractcanmovesq(u32arm_t* const d, u32arm_t value)
{
    u32arm_t i, j;

    for (i = 0, j = 0; i < 4; ++i) {
        if (value & (1 << i))
            d[j++] = i;
    }

    return j;
}

u32arm_t randomsq(u32arm_t* const sq, u32arm_t index, u32arm_t hw, u32arm_t* seed)
{
    u32arm_t i, d[4], j;


    for (i = 0; i < RANDLOOP; ++i) {
        j = extractcanmovesq(d, canmovesq(sq, index, hw));
        slide(sq, d[minstd_rand(seed) % j], index, hw);
    }

    return RANDLOOP;
}

u32arm_t gameid(const u32arm_t* const sq, u32arm_t hw)
{
    u32arm_t i;

    for (i = 0; i < hw * hw; ++i)
        if (sq[i] != i)
            return gid_normal;

    return gid_over;
}

