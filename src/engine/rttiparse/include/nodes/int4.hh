/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_NODES_INT4_HH_
#define BE_RTTIPARSE_NODES_INT4_HH_
/**************************************************************************************************/
#include <bugengine/rttiparse/stdafx.h>
#include <bugengine/kernel/simd.hh>
#include <bugengine/rttiparse/valueparse.hh>

namespace BugEngine { namespace RTTI { namespace Parser {

class Int4 : public Node
{
private:
    const bigint4 m_value;

protected:
    virtual bool isCompatible(const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;

public:
    Int4(const ParseLocation& location, bigint4 value);
    ~Int4();
};

}}}  // namespace BugEngine::RTTI::Parser

/**************************************************************************************************/
#endif
