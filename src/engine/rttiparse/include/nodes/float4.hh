/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_NODES_FLOAT4_HH_
#define BE_RTTIPARSE_NODES_FLOAT4_HH_
/**************************************************************************************************/
#include    <rttiparse/stdafx.h>
#include    <rttiparse/valueparse.hh>
#include    <kernel/simd.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

class Float4 : public Node
{
private:
    const float4    m_value;
protected:
    virtual bool isCompatible(const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;
public:
    Float4(const ParseLocation& location, float4 value);
    ~Float4();
};

}}}

/**************************************************************************************************/
#endif
