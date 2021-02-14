/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.scripting.package/stdafx.h>
#include <buildcontext.hh>

int be_package_lex_destroy();

const minitl::Allocator::Block< u8 >* g_buffer                   = 0;
int                                   g_bufferPosition           = 0;
int                                   g_packageOffset            = 0;
int                                   g_packageLine              = 0;
int                                   g_packageColumnBefore      = 0;
int                                   g_packageColumnAfter       = 0;
int                                   g_packageObjectNestedLevel = 0;
static i_u32                          s_useCount                 = i_u32::create(0);

namespace BugEngine { namespace PackageBuilder {

BuildContext::BuildContext(const ifilename& filename, const minitl::Allocator::Block< u8 >& buffer,
                           ref< Folder > folder)
    : result(ref< Nodes::Package >::create(Arena::packageBuilder(), filename, folder))
{
    be_assert(s_useCount++ == 0, "non reentrant parser used by two threads");
    g_buffer              = &buffer;
    g_bufferPosition      = 0;
    g_packageOffset       = 0;
    g_packageLine         = 0;
    g_packageColumnBefore = g_packageColumnAfter = 0;
    g_packageObjectNestedLevel                   = 0;
}

BuildContext::~BuildContext()
{
    be_package_lex_destroy();
    g_buffer         = 0;
    g_bufferPosition = 0;
    --s_useCount;
}

}}  // namespace BugEngine::PackageBuilder
