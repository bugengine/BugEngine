/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_DBNAMESPACE_HH_
#define BE_RTTIPARSE_DBNAMESPACE_HH_
/**************************************************************************************************/
#include    <rttiparse/stdafx.h>
#include    <rtti/value.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

class Node;
struct DbContext;

class Namespace : public minitl::refcountable
{
private:
    minitl::Allocator&                          m_allocator;
    minitl::hashmap<istring, ref<Namespace> >   m_children;
    minitl::hashmap<istring, ref<Node> >        m_nodes;
    Value                                       m_value;
public:
    Namespace(minitl::Allocator& allocator);
    Namespace(minitl::Allocator& allocator, const Value& value);
    ~Namespace();

    void add(DbContext& context, const inamespace& name, const Value& value);
    void add(DbContext& context, const inamespace& name, ref<Namespace> ns);
    void add(DbContext& context, const inamespace& name, ref<Node> node);
    ref<Namespace> getChild(const istring name) const;
    ref<Node> getNode(const istring name) const;
    const Value& getValue() const;
};

}}}


/**************************************************************************************************/
#endif

