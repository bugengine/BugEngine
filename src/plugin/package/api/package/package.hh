/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGE_PACKAGE_HH_
#define BE_PACKAGE_PACKAGE_HH_
/*****************************************************************************/
#include    <core/endianness.hh>

namespace BugEngine { namespace PackageManager
{

struct PackageHeader
{
    u32_l   objectCount;
};

}}

/*****************************************************************************/
#endif
