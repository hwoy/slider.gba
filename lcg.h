#ifndef _LCG_H_
#define _LCG_H_

#include "arm7type.hpp"

u32arm_t lcg_lehmer(u32arm_t* sd, u32arm_t a, u32arm_t m);

u32arm_t lcg(u32arm_t* sd, u32arm_t a, u32arm_t c, u32arm_t m);

u32arm_t lcg_glibc(u32arm_t* sd);

u32arm_t lcg_ms(u32arm_t* sd);

#endif
