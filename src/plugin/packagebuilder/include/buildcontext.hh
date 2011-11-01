/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGEBUILDER_BUILDCONTEXT_HH_
#define BE_PACKAGEBUILDER_BUILDCONTEXT_HH_
/*****************************************************************************/
#include    <system/plugin.hh>
#include    <packagebuilder/nodes/package.hh>

namespace BugEngine { namespace PackageBuilder
{

struct BuildContext
{
    minitl::hashmap< istring, Plugin<minitl::pointer> > plugins;
    minitl::hashmap< istring, const RTTI::ClassInfo* >  imports;
    minitl::vector< Value >                             objects;
    ref<Nodes::Package>                                 result;

    BuildContext()
        :   plugins(tempArena())
        ,   imports(tempArena())
        ,   objects(tempArena())
        ,   result()
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
