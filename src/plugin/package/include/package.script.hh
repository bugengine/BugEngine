/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_PACKAGE_SCRIPT_HH_
#define BE_RTTI_PACKAGE_SCRIPT_HH_
/*****************************************************************************/
#include    <system/resource/resource.script.hh>

namespace BugEngine { namespace Plugin { namespace PackageManager
{

be_tag(ResourceLoaders())
class Package : public Resource
{
published:
    Package(const ifilename& filename);
    ~Package();
};

}}}

/*****************************************************************************/
#endif
