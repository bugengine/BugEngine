/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_NODE_HH_
#define BE_RTTIPARSE_NODE_HH_
/**************************************************************************************************/
#include    <rttiparse/stdafx.h>
#include    <rtti/value.hh>
#include    <rttiparse/dbcontext.hh>
#include    <rttiparse/dbnamespace.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

class Array;
class be_api(RTTI) Node : public minitl::refcountable
{
    friend class Array;
protected:
    ParseLocation   m_location;
private:
    mutable Value   m_cache;
    mutable bool    m_cacheSet;
protected:
    Node(const ParseLocation& location)
        :   m_location(location)
        ,   m_cache()
        ,   m_cacheSet(false)
    {
    }
    virtual void doEval(const Type& expectedType, Value& result) const = 0;
public:
    virtual bool isCompatible(const Type& expectedType) const = 0;
    virtual bool resolve(DbContext& context);
    void eval(DbContext &context, const Type &expectedType, Value& result) const;
    Value eval(DbContext& context, const Type& expectedType) const;
};

}}}

/**************************************************************************************************/
#endif

