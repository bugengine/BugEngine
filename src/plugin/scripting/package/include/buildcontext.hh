/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PACKAGEBUILDER_BUILDCONTEXT_HH_
#define BE_PACKAGEBUILDER_BUILDCONTEXT_HH_
/**************************************************************************************************/
#include    <package/stdafx.h>
#include    <plugin/plugin.hh>
#include    <package/nodes/package.hh>
#include    <package/nodes/value.hh>
#include    <package/nodes/entity.hh>
#include    <filesystem/folder.script.hh>

union YYSTYPE
{
    bool bValue;
    i64 iValue;
    double fValue;
    char* sValue;
    ref<BugEngine::PackageBuilder::Nodes::Value>* value;
    ref<BugEngine::PackageBuilder::Nodes::Parameter>* param;
    ref<BugEngine::PackageBuilder::Nodes::Object>* object;
    ref<BugEngine::PackageBuilder::Nodes::Component>* component;
    ref<BugEngine::PackageBuilder::Nodes::Entity>* entity;
    minitl::vector< ref<BugEngine::PackageBuilder::Nodes::Value> >* value_list;
    minitl::vector< ref<BugEngine::PackageBuilder::Nodes::Parameter> >* param_list;
    minitl::vector< ref<BugEngine::PackageBuilder::Nodes::Component> >* component_list;
    minitl::vector< ref<BugEngine::PackageBuilder::Nodes::Entity> >* entity_list;
};
#define YYSTYPE_IS_DECLARED 1
#define YYSTYPE_IS_TRIVIAL 1



namespace BugEngine { namespace PackageBuilder
{

struct BuildContext
{
    ref<Nodes::Package> result;
    ref<Folder> folder;

    BuildContext(const ifilename& filename,
                 const minitl::Allocator::Block<u8>& buffer,
                 ref<Folder> folder);
    ~BuildContext();
};

}}

extern int g_packageLine;
extern int g_packageColumnBefore;
extern int g_packageColumnAfter;

extern const minitl::Allocator::Block<u8>* g_buffer;
extern int g_bufferPosition;

/**************************************************************************************************/
#endif
