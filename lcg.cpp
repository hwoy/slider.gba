#include "lcg.h"
#include "arm7type.hpp"

u32arm_t lcg_lehmer(u32arm_t* sd, u32arm_t a, u32arm_t m)
{
    u32arm_t hi;
    u32arm_t lo;

    hi = *sd / (m / a);
    lo = *sd % (m / a);

    *sd = a * lo - (m % a) * hi;

    return (i32arm_t)*sd <= 0 ? *sd += m : *sd;
}

u32arm_t lcg(u32arm_t* sd, u32arm_t a, u32arm_t c, u32arm_t m)
{
    return *sd = (*sd * a + c) & m;
}

u32arm_t lcg_glibc(u32arm_t* sd)
{
    return lcg(sd, 1103515245, 12345, (1U << 31) - 1);
}

u32arm_t lcg_ms(u32arm_t* sd)
{
    return lcg(sd, 214013, 2531011, (1U << 31) - 1) >> 16;
}
