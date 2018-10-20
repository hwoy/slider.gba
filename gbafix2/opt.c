#include "opt.h"
#include <string.h>

static int i_upindex = DUPINDEX;

int opt_action(int argc, const char* const argv[], const char* const param[], char* const buff,
    unsigned int bsize, int start)
{
    int i, j, l;
    unsigned int k;

    buff[0] = 0;

    for (j = (i_upindex >= 0) ? i_upindex : start; j < argc; j++) {

        for (i = 0; param[i]; i++) {

            if (!(strncmp(argv[j], param[i], strlen(param[i])))) {
                for (k = 0, l = strlen(param[i]);
                     argv[j][l]; k++, l++) {
                    if (k + 1 >= bsize)
                        return e_optoom;
                    buff[k] = argv[j][l];
                }
                buff[k] = 0;
                i_upindex = j + 1;
                return i;
            }
        }

        for (k = 0, l = 0; argv[j][l]; k++, l++) {
            if (k + 1 >= bsize)
                return e_optoom;
            buff[k] = argv[j][l];
        }
        buff[k] = 0;
        i_upindex = j + 1;
        return e_optother;
    }
    return e_optend;
}

int opt_getIndex(void)
{
    return i_upindex;
}

void opt_setIndex(int index)
{
    i_upindex = index;
}

void opt_resetIndex(void)
{
    i_upindex = DUPINDEX;
}
