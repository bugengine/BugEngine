/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGE_PACKAGE_SCRIPT_HH_
#define BE_PACKAGE_PACKAGE_SCRIPT_HH_
/*****************************************************************************/
#include    <bugengine/script.script.hh>
#include    <rtti/tags/editor.script.hh>

namespace BugEngine
{

class PackageLoader;

class be_api(PACKAGE) Package : public Script
{
    friend class PackageLoader;
published:
    Package(weak<const File> file);
    ~Package();
};

}

/*****************************************************************************/
#endif
