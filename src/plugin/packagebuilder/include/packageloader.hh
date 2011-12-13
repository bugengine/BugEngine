/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGEBUILDER_PACKAGELOADER_HH_
#define BE_PACKAGEBUILDER_PACKAGELOADER_HH_
/*****************************************************************************/
#include    <packagebuilder/package.script.hh>
#include    <bugengine/scriptengine.hh>

namespace BugEngine { namespace PackageBuilder
{

class PackageLoader : public ScriptEngine<Package>
{
    BE_NOCOPY(PackageLoader);
public:
    PackageLoader();
    ~PackageLoader();
private:
    virtual void runBuffer(weak<const Package> script, const Allocator::Block<u8>& buffer) override;
};

}}

/*****************************************************************************/
#endif
