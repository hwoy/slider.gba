#include "function.h"

int isUint(const char* const ch)
{
    unsigned int i;
    if (!sLen(ch))
        return 0;
    for (i = 0; ch[i]; i++) {
        if (ch[i] < '0' || ch[i] > '9') {
            return 0;
        }
    }
    return 1;
}

unsigned int
pow2ui(unsigned int base, unsigned int pow)
{
    unsigned int i, j;
    if (!base)
        return 0;
    if (!pow)
        return 1;
    for (i = 1, j = base; i < pow; i++) {
        j = j * base;
    }
    return j;
}

unsigned int
s2ui(const char* const ch)
{
    unsigned int i, j, k;
    j = 0;
    k = 0;
    i = sLen(ch) - 1;
    do {
        j += (ch[i] - '0') * pow2ui(10, k++);
    } while (i-- != 0);

    return j;
}

unsigned int
sLen(const char* const ch)
{
    unsigned int i;

    for (i = 0; ch[i]; i++)
        ;

    return i;
}
