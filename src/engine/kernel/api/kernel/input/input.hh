/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_INPUT_INPUT_HH_
#define BE_KERNEL_INPUT_INPUT_HH_
/**************************************************************************************************/
#define BE_INCLUDE_INPUT_cpp <kernel/input/cpp/input.hh>
#define BE_(x) x

#define BE_INCLUDE_INPUT_CAT_(x) BE_(BE_INCLUDE_INPUT_##x)
#define BE_INCLUDE_INPUT_CAT(x) BE_INCLUDE_INPUT_CAT_(x)
#include BE_INCLUDE_INPUT_CAT(BE_KERNEL_TARGET)

/**************************************************************************************************/
#endif
