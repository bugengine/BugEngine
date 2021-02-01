/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_AST_NODE_HH_
#define BE_RTTI_AST_NODE_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/dbcontext.hh>
#include <bugengine/rtti-ast/dbnamespace.hh>
#include <bugengine/rtti/value.hh>

namespace BugEngine { namespace RTTI { namespace AST {

class Array;
class be_api(RTTI_AST) Node : public minitl::refcountable
{
    friend class Array;

protected:
    ParseLocation m_location;

private:
    mutable Value m_cache;
    mutable bool  m_cacheSet;

protected:
    Node(const ParseLocation& location) : m_location(location), m_cache(), m_cacheSet(false)
    {
    }
    virtual void doEval(const Type& expectedType, Value& result) const = 0;

public:
    virtual bool isCompatible(const Type& expectedType) const = 0;
    virtual bool resolve(DbContext & context);
    void         eval(DbContext & context, const Type& expectedType, Value& result) const;
    Value        eval(DbContext & context, const Type& expectedType) const;
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
