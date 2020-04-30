/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_NODES_BOOL_HH_
#define BE_RTTIPARSE_NODES_BOOL_HH_
/**************************************************************************************************/
#include <bugengine/rttiparse/stdafx.h>
#include <bugengine/rttiparse/valueparse.hh>

namespace BugEngine { namespace RTTI { namespace Parser {

class Bool : public Node
{
private:
    const bool m_value;

protected:
    virtual bool isCompatible(const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;

public:
    Bool(const ParseLocation& location, bool value);
    ~Bool();
};

}}}  // namespace BugEngine::RTTI::Parser

/**************************************************************************************************/
#endif
