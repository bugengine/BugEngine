/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGEBUILDER_BUILDCONTEXT_HH_
#define BE_PACKAGEBUILDER_BUILDCONTEXT_HH_
/*****************************************************************************/
#include    <system/plugin.hh>
#include    <package/nodes/package.hh>
#include    <package/nodes/value.hh>
#include    <system/file/folder.script.hh>

union YYSTYPE
{
    bool bValue;
    i64 iValue;
    double fValue;
    char* sValue;
    ref<BugEngine::PackageBuilder::Nodes::Value>* value;
};
#define YYSTYPE_IS_DECLARED 1
#define YYSTYPE_IS_TRIVIAL 1



namespace BugEngine
{
namespace PackageBuilder
{

struct BuildContext
{
    ref<Nodes::Package> result;
    ref<Folder> folder;

    BuildContext(const Allocator::Block<u8>& buffer, ref<Folder> folder);
    ~BuildContext();
};

}
}

extern int g_packageLine;
extern int g_packageColumnBefore;
extern int g_packageColumnAfter;

extern const BugEngine::Allocator::Block<u8>* g_buffer;
extern int g_bufferPosition;

/*****************************************************************************/
#endif
