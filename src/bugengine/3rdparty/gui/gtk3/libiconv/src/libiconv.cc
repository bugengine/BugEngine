#ifdef _MSC_VER
/* some wchar functions seem to only be declared for C++. Include cwchar to drag them in */
#   include <cwchar>
/* old MSVC seem to export C functions only in the C++ library. This include will force to link with the C++ library */
#   include <iostream>
#endif

#include "iconv.c"
