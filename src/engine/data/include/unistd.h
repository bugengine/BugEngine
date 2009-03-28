#ifndef _UNISTD_H_
#define _UNISTD_H_

static inline int isatty(int desc)
{
    (void)desc;
    return 0;
}

#endif

