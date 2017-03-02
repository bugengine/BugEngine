/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_VALUEPARSE_HH_
#define BE_RTTIPARSE_VALUEPARSE_HH_
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
    virtual Type getType() const = 0;
    virtual bool isCompatible(const Type& expectedType) const = 0;
    virtual bool resolve(DbContext& context)
    {
        be_forceuse(context);
        return true;
    }
    void eval(DbContext &context, const Type &expectedType, Value& result) const
    {
        if (!m_cacheSet)
        {
            if (!isCompatible(expectedType))
            {
                context.error(m_location,
                              ErrorType("type mismatch; expected %s") | expectedType.name());
            }
            else
            {
                doEval(expectedType, result);
                m_cache = Value(Value::ByRef(result));
            }
            m_cacheSet = true;
        }
    }
    Value eval(DbContext& context, const Type& expectedType) const
    {
        if (!m_cacheSet)
        {
            if (!isCompatible(expectedType))
            {
                context.error(m_location,
                              ErrorType("type mismatch; expected %s") | expectedType.name());
            }
            else
            {
                doEval(expectedType, m_cache);
            }
            m_cacheSet = true;
        }
        return Value(Value::ByRef(m_cache));
    }
};

}

be_api(RTTI)
Value parseValue(minitl::Allocator& arena,
                 Parser::DbContext& context,
                 const char* strBegin,
                 const char *strEnd = 0,
                 u32 initialLine = 0,
                 u32 initialColumn = 0);
be_api(RTTI)
Value parseValue(minitl::Allocator& arena,
                 Parser::DbContext& context,
                 const Type& expectedType,
                 const char* strBegin,
                 const char *strEnd = 0,
                 u32 initialLine = 0,
                 u32 initialColumn = 0);
be_api(RTTI)
ref<Parser::Node> buildParseTree(minitl::Allocator& arena,
                                 Parser::ErrorList& errors,
                                 const char* strBegin,
                                 const char *strEnd = 0,
                                 u32 initialLine = 0,
                                 u32 initialColumn = 0);

}}

/**************************************************************************************************/
#endif

