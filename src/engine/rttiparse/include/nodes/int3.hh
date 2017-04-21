/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_NODES_INT3_HH_
#define BE_RTTIPARSE_NODES_INT3_HH_
/**************************************************************************************************/
#include    <rttiparse/stdafx.h>
#include    <rttiparse/valueparse.hh>
#include    <kernel/simd.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

class Int3 : public Node
{
private:
    const bigint3   m_value;
protected:
    virtual bool isCompatible(const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;
public:
    Int3(const ParseLocation& location, bigint3 value);
    ~Int3();
};

}}}

/**************************************************************************************************/
#endif
