/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGEBUILDER_BUILDCONTEXT_HH_
#define BE_PACKAGEBUILDER_BUILDCONTEXT_HH_
/*****************************************************************************/
#include    <plugin/plugin.hh>
#include    <package/nodes/package.hh>
#include    <package/nodes/value.hh>
#include    <filesystem/folder.script.hh>

union YYSTYPE
{
    bool bValue;
    i64 iValue;
    double fValue;
    char* sValue;
    ref<BugEngine::PackageBuilder::Nodes::Value>* value;
    minitl::vector< ref<BugEngine::PackageBuilder::Nodes::Value> >* array;
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

    BuildContext(const minitl::Allocator::Block<u8>& buffer, ref<Folder> folder);
    ~BuildContext();
};

}
}

extern int g_packageLine;
extern int g_packageColumnBefore;
extern int g_packageColumnAfter;

extern const minitl::Allocator::Block<u8>* g_buffer;
extern int g_bufferPosition;

/*****************************************************************************/
#endif
