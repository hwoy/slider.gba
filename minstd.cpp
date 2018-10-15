#include "minstd.h"
#include "arm7type.hpp"

u32arm_t minstd_rand0(u32arm_t* sd)
{
    return lcg_lehmer(sd, Astd0, Mstd);
}

u32arm_t minstd_rand(u32arm_t* sd)
{
    return lcg_lehmer(sd, Astd, Mstd);
}
