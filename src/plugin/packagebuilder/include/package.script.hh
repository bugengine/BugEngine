/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_PACKAGEBUILDER_SCRIPT_HH_
#define BE_RTTI_PACKAGEBUILDER_SCRIPT_HH_
/*****************************************************************************/
#include    <system/resource/resource.script.hh>
#include    <system/file/file.script.hh>
#include    <rtti/tags/editor.script.hh>

namespace BugEngine { namespace PackageBuilder
{

class PackageLoader;

be_tag(ResourceLoaders())
class Package : public Resource
{
    friend class PackageLoader;
private:
    ref<const File> m_packageFile;
published:
    Package(be_tag(EditHint::Extension(".pkg")) ref<const File> file);
    ~Package();
};

}}

/*****************************************************************************/
#endif
