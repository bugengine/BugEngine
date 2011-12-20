/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGEBUILDER_BUILDCONTEXT_HH_
#define BE_PACKAGEBUILDER_BUILDCONTEXT_HH_
/*****************************************************************************/
#include    <system/plugin.hh>
#include    <package/nodes/package.hh>

namespace BugEngine { namespace PackageBuilder
{

struct BuildContext
{
    ref<Nodes::Package>                                 result;

    BuildContext()
        :   result(ref<Nodes::Package>::create(packageBuilderArena()))
    {
    }
};

}}

extern int g_packageLine;
extern int g_packageColumnBefore;
extern int g_packageColumnAfter;

extern const BugEngine::Allocator::Block<u8>* g_buffer;
extern int g_bufferPosition;

/*****************************************************************************/
#endif
