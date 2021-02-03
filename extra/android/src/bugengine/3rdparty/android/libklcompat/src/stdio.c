#include <stdio.h>


#if __ANDROID_API__ < 23
extern FILE __sF[];

#undef stdin
#undef stdout
#undef stderr

__attribute__((visibility("default")))
FILE* stdin = (&__sF[2]);

__attribute__((visibility("default")))
FILE* stdout = (&__sF[1]);

__attribute__((visibility("default")))
FILE* stderr = (&__sF[2]);
#endif
