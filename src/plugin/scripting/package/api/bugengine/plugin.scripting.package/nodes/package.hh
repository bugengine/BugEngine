/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_PACKAGE_HH_
#define BE_PACKAGE_NODES_PACKAGE_HH_
/**************************************************************************************************/
#include <bugengine/plugin.scripting.package/stdafx.h>

#include <bugengine/minitl/hash_map.hh>
#include <bugengine/minitl/intrusive_list.hh>
#include <bugengine/minitl/vector.hh>
#include <bugengine/plugin.scripting.package/logger.hh>
#include <bugengine/plugin/plugin.hh>
#include <bugengine/rtti-ast/dbcontext.hh>
#include <bugengine/rtti-ast/node/node.hh>
#include <bugengine/rtti/value.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes {

class Package : public minitl::refcountable
{
private:
    const ifilename                           m_filename;
    RTTI::AST::DbContext                      m_context;
    minitl::vector< Plugin::Plugin< void* > > m_plugins;
    minitl::vector< ref< RTTI::AST::Node > >  m_nodes;
    minitl::vector< RTTI::Value >             m_values;
    RTTI::Value                               m_empty;
    Logger                                    m_logger;

public:
    Package(const ifilename& filename);
    ~Package();

    void                   insertNode(const istring name, ref< RTTI::AST::Node > object);
    void                   removeNode(ref< RTTI::AST::Node > object);
    ref< RTTI::AST::Node > findByName(istring name) const;
    const RTTI::Value&     getValue(weak< const RTTI::AST::Node > object) const;

    void loadPlugin(inamespace plugin, inamespace name);

    void binarySave() const;
    void textSave() const;

    void createObjects(weak< Resource::ResourceManager > manager);
    void deleteObjects(weak< Resource::ResourceManager > manager);
    void diffFromPackage(weak< Package > previous, weak< Resource::ResourceManager > manager);

    const ifilename& filename() const;

    void info(u32 line, const char* message);
    void warning(u32 line, const char* message);
    void error(u32 line, const char* message);

    bool success() const;

    const RTTI::AST::DbContext& context() const
    {
        return m_context;
    }

    RTTI::AST::DbContext& context()
    {
        return m_context;
    }
};

}}}  // namespace BugEngine::PackageBuilder::Nodes

/**************************************************************************************************/
#endif
