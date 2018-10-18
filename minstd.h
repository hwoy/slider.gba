#ifndef _MINSTD_H_
#define _MINSTD_H_

#include "arm7type.h"
#include "lcg.h"

#define Astd0 16807
#define Cstd 0
#define Mstd 2147483647U

#define Astd 48271

#ifdef __cplusplus
extern "C" {
#endif

u32arm_t minstd_rand0(u32arm_t* sd);

u32arm_t minstd_rand(u32arm_t* sd);

#ifdef __cplusplus
}
#endif

#endif
