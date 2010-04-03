/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_SCHEDULER_TASK_METHOD_HH_
#define BE_SYSTEM_SCHEDULER_TASK_METHOD_HH_
/*****************************************************************************/
#include    <system/scheduler/task/task.hh>
#include    <system/scheduler/range/onestep.hh>

namespace BugEngine
{

template< typename Owner, void(Owner::*Method)(), template< typename > class Ptr = weak >
struct MethodCaller
{
private:
    Ptr<Owner> const m_owner;
private:
    MethodCaller& operator=(const MethodCaller& other);
public:
    typedef range_onestep   Range;
    MethodCaller(Ptr<Owner> owner)
        :   m_owner(owner)
    {
    }
    ~MethodCaller()
    {
    }

    range_onestep prepare() { return range_onestep(); }
    void operator()(const range_onestep& /*r*/) const
    {
        (m_owner.operator->()->*Method)();
    }
    void operator()(range_onestep& /*myRange*/, MethodCaller& /*with*/, range_onestep& /*withRange*/)
    {
    }
};

template< void(*Procedure)() >
struct ProcedureCaller
{
private:
    ProcedureCaller& operator=(const ProcedureCaller& other);
public:
    typedef range_onestep   Range;
    ProcedureCaller()
    {
    }
    ~ProcedureCaller()
    {
    }

    range_onestep prepare() { return range_onestep(); }
    void operator()(const range_onestep& /*r*/) const
    {
        (*Procedure)();
    }
    void operator()(range_onestep& /*myRange*/, ProcedureCaller& /*with*/, range_onestep& /*withRange*/)
    {
    }
};

}

/*****************************************************************************/
#endif
