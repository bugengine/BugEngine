/* old MSVC seem to export C functions only in the C++ library. This file will force to link with the C++ library */
#ifdef _MSC_VER
#   include <cwchar>
#endif

#include "lib/iconv.c"
#include "lib/relocatable.c"
#include "libcharset/lib/localcharset.c"