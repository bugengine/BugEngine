/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_DBNAMESPACE_HH_
#define BE_RTTI_AST_DBNAMESPACE_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/node.hh>
#include <bugengine/rtti/value.hh>

namespace BugEngine { namespace RTTI { namespace AST {

class be_api(RTTI_AST) Namespace : public minitl::refcountable
{
private:
    minitl::Allocator&                           m_allocator;
    minitl::hashmap< istring, ref< Namespace > > m_children;
    minitl::hashmap< istring, ref< Node > >      m_nodes;
    Value                                        m_value;

public:
    Namespace(minitl::Allocator & allocator);
    Namespace(minitl::Allocator & allocator, const Value& value);
    ~Namespace();

    void             add(const inamespace& name, const Value& value);
    void             add(const inamespace& name, ref< Namespace > ns);
    void             add(const inamespace& name, ref< Node > node);
    void             remove(const inamespace& name, ref< Node > node);
    ref< Namespace > getChild(const istring name) const;
    ref< Node >      getNode(const istring name) const;
    const Value&     getValue() const;
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
