/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_PACKAGE_SCRIPT_HH_
#define BE_RTTI_PACKAGE_SCRIPT_HH_
/*****************************************************************************/
#include    <bugengine/script.script.hh>
#include    <rtti/tags/editor.script.hh>

namespace BugEngine { namespace PackageManager
{

class PackageLoader;

class Package : public Script
{
    friend class PackageLoader;
published:
    Package(be_tag(EditHint::Extension(".bpkg")) ref<const File> file);
    ~Package();
};

}}

/*****************************************************************************/
#endif
