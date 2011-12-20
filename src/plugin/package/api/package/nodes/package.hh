/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_PACKAGE_HH_
#define BE_PACKAGE_NODES_PACKAGE_HH_
/*****************************************************************************/
#include    <system/plugin.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

class Object;
class Reference;

class Package : public minitl::refcountable
{
    friend class Reference;
private:
    minitl::vector< Plugin<void*> >     m_plugins;
    minitl::hashmap< istring, Value >   m_imports;
    minitl::vector< ref<Object> >       m_nodes;
    minitl::intrusive_list<Reference>   m_references;
private:
    void addReference(weak<Reference> reference);
    void resolveReference(weak<Reference> reference);
public:
    Package();
    ~Package();

    void insertNode(ref<Object> object);
    void removeNode(ref<Object> object);
    ref<Object> findByName(istring name) const;

    void loadPlugin(istring);

    void binarySave() const;
    void textSave() const;
};

}}}

/*****************************************************************************/
#endif
