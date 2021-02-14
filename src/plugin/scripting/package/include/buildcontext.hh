/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGEBUILDER_BUILDCONTEXT_HH_
#define BE_PACKAGEBUILDER_BUILDCONTEXT_HH_
/**************************************************************************************************/
#include <bugengine/plugin.scripting.package/stdafx.h>
#include <bugengine/plugin.scripting.package/nodes/package.hh>

#include <bugengine/plugin/plugin.hh>
#include <bugengine/rtti-ast/node/node.hh>

struct FileOffset
{
    int line;
    int column;
    int start;
    int end;
};

union YYSTYPE
{
    const char* id;
    FileOffset  offset;
};

#define YYSTYPE_IS_DECLARED 1
#define YYSTYPE_IS_TRIVIAL  1

namespace BugEngine { namespace PackageBuilder {

struct BuildContext
{
    ref< Nodes::Package > result;

    BuildContext(const ifilename& filename, const minitl::Allocator::Block< u8 >& buffer,
                 ref< Folder > folder);
    ~BuildContext();
};

}}  // namespace BugEngine::PackageBuilder

extern int g_packageOffset;
extern int g_packageLine;
extern int g_packageColumnBefore;
extern int g_packageColumnAfter;
extern int g_packageObjectNestedLevel;

extern const minitl::Allocator::Block< u8 >* g_buffer;
extern int                                   g_bufferPosition;

/**************************************************************************************************/
#endif
