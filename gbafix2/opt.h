#ifndef _OPT_H
#define _OPT_H

#define DSTART 1
#define DUPINDEX -1

enum __opt__ {
    e_optother = -1,
    e_optend = -2,
    e_optoom = -3
};

int opt_action(int argc, const char* const argv[], const char* const param[], char* const buff,
    unsigned int bsize, int start);

int opt_getIndex(void);

void opt_setIndex(int index);

void opt_resetIndex(void);

#endif
