/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PACKAGEBUILDER_BUILDCONTEXT_HH_
#define BE_PACKAGEBUILDER_BUILDCONTEXT_HH_
/**************************************************************************************************/
#include <bugengine/plugin.scripting.package/stdafx.h>
#include <bugengine/plugin.scripting.package/nodes/package.hh>

#include <bugengine/plugin/plugin.hh>
#include <bugengine/rtti-ast/node/node.hh>

union YYSTYPE
{
    char*                              sValue;
    ref< BugEngine::RTTI::AST::Node >* value;
};
#define YYSTYPE_IS_DECLARED 1
#define YYSTYPE_IS_TRIVIAL  1

namespace BugEngine { namespace PackageBuilder {

struct BuildContext
{
    ref< Nodes::Package > result;
    ref< Folder >         folder;

    BuildContext(const ifilename& filename, const minitl::Allocator::Block< u8 >& buffer,
                 ref< Folder > folder);
    ~BuildContext();
};

}}  // namespace BugEngine::PackageBuilder

extern int g_packageLine;
extern int g_packageColumnBefore;
extern int g_packageColumnAfter;
extern int g_packageObjectNestedLevel;

extern const minitl::Allocator::Block< u8 >* g_buffer;
extern int                                   g_bufferPosition;

/**************************************************************************************************/
#endif
