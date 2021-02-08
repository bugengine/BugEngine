/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_NODE_HH_
#define BE_RTTI_AST_NODE_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti/value.hh>

namespace BugEngine { namespace RTTI { namespace AST {

class Array;
class Parameter;
struct DbContext;

class be_api(RTTI_AST) Node : public minitl::refcountable
{
    friend class Array;
    friend class Parameter;

private:
    minitl::vector< minitl::tuple< const istring, RTTI::Value > > m_metadata;
    mutable Value                                                 m_cache;
    mutable bool                                                  m_cacheSet;

protected:
    Node() : m_metadata(Arena::rtti()), m_cache(), m_cacheSet(false)
    {
    }
    virtual void doEval(const Type& expectedType, Value& result) const = 0;

public:
    virtual bool isCompatible(const Type& expectedType) const = 0;
    virtual bool resolve(DbContext & context);
    void         eval(DbContext & context, const Type& expectedType, Value& result) const;
    Value        eval(DbContext & context, const Type& expectedType) const;

    template < typename T >
    void setMetadata(const istring name, T value)
    {
        for(minitl::vector< minitl::tuple< const istring, RTTI::Value > >::iterator it
            = m_metadata.begin();
            it != m_metadata.end(); ++it)
        {
            if(it->first == name)
            {
                it->second = value;
                return;
            }
        }
        m_metadata.push_back(minitl::make_tuple(name, RTTI::Value()));
        m_metadata.rbegin()->second = value;
    }
    const Value& getMetadata(const istring name) const;
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
