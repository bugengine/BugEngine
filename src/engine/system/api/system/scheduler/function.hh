/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_FUNCTION_HH_
#define BE_SYSTEM_SCHEDULER_FUNCTION_HH_
/*****************************************************************************/
#include    <system/scheduler/task.hh>
#include    <system/scheduler/range/onestep.hh>

namespace BugEngine
{

template< typename Owner, void(Owner::*Function)() >
class FunctionBody
{
    friend class Task<FunctionBody>;
private:
    typedef range_onestep   Range;
    weak<Owner>             m_owner;
public:
    FunctionBody(weak<Owner> owner)
        :   m_owner(owner)
    {
    }
    ~FunctionBody()
    {
    }

    range_onestep prepare() { return range_onestep(); }
    void operator()(range_onestep& /*r*/)
    {
        (m_owner.operator->()->*Function)();
    }
    void operator()(range_onestep& /*myRange*/, FunctionBody& /*with*/, range_onestep& /*withRange*/)
    {
    }
};

}

/*****************************************************************************/
#endif
