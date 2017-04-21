/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_NODES_INTEGER_HH_
#define BE_RTTIPARSE_NODES_INTEGER_HH_
/**************************************************************************************************/
#include    <rttiparse/stdafx.h>
#include    <rttiparse/valueparse.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

class Integer : public Node
{
private:
    const i64   m_value;
protected:
    virtual bool isCompatible(const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;
public:
    Integer(const ParseLocation& location, i64 value);
    ~Integer();
};

}}}

/**************************************************************************************************/
#endif
