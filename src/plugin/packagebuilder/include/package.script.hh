/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAHEBUILDER_PACKAGE_SCRIPT_HH_
#define BE_PACKAHEBUILDER_PACKAGE_SCRIPT_HH_
/*****************************************************************************/
#include    <main/script.script.hh>
#include    <rtti/tags/editor.script.hh>

namespace BugEngine { namespace PackageBuilder
{

be_tag(ResourceLoaders())
class Package : public Script
{
    friend class PackageLoader;
published:
    Package(be_tag(EditHint::Extension(".pkg")) ref<const File> file);
    ~Package();
};

}}

/*****************************************************************************/
#endif
