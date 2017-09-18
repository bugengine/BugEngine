/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_PACKAGE_HH_
#define BE_PACKAGE_NODES_PACKAGE_HH_
/**************************************************************************************************/
#include    <package/stdafx.h>
#include    <plugin/plugin.hh>
#include    <minitl/vector.hh>
#include    <minitl/intrusive_list.hh>
#include    <minitl/hash_map.hh>
#include    <package/logger.hh>
#include    <rtti/value.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

class Object;
class Reference;

class Package : public minitl::refcountable
{
    friend class Reference;
private:
    class Namespace : public minitl::refcountable
    {
    private:
        minitl::hashmap<istring, RTTI::Value>  m_values;
        minitl::hashmap<istring, ref<Namespace> >   m_children;
    public:
        Namespace();
        ~Namespace();

        RTTI::Value get(const inamespace& name) const;
        void add(const inamespace& name, const RTTI::Value& value);
    };
private:
    const ifilename                         m_filename;
    minitl::vector< Plugin::Plugin<void*> > m_plugins;
    ref<Namespace>                          m_rootNamespace;
    minitl::vector< ref<Object> >           m_nodes;
    minitl::intrusive_list<Reference>       m_references;
    minitl::vector<RTTI::Value>             m_values;
    RTTI::Value                             m_empty;
    Logger                                  m_logger;
private:
    void addReference(weak<Reference> reference);
    void resolveReference(weak<Reference> reference);
public:
    Package(const ifilename& filename);
    ~Package();

    void insertNode(ref<Object> object);
    void removeNode(ref<Object> object);
    ref<Object> findByName(istring name) const;
    const RTTI::Value& getValue(weak<const Object> object) const;

    void loadPlugin(inamespace plugin, inamespace name);

    void binarySave() const;
    void textSave() const;

    void createObjects(weak<Resource::ResourceManager> manager);
    void deleteObjects(weak<Resource::ResourceManager> manager);
    void diffFromPackage(weak<Package> previous, weak<Resource::ResourceManager> manager);

    const ifilename& filename() const;

    void info(u32 line, const char* message);
    void warning(u32 line, const char* message);
    void error(u32 line, const char* message);

    bool success() const;
};

}}}

/**************************************************************************************************/
#endif
