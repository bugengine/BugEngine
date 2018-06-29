/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETER_SEGMENT_FACTORY_HH_
#define BE_SCHEDULER_KERNEL_PARAMETER_SEGMENT_FACTORY_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <scheduler/kernel/parameters/segment.hh>
#include    <rtti/typeinfo.hh>
#include    <rtti/classinfo.script.hh>

namespace BugEngine
{

template< typename T >
struct be_typeid<  KernelScheduler::Segment<T> >
{
    static BE_EXPORT istring name();
    static BE_EXPORT raw<const RTTI::Class> klass();
    static BE_EXPORT RTTI::Type  type();
};

template< typename T >
BE_EXPORT
istring be_typeid< KernelScheduler::Segment<T> >::name()
{
    static istring s_result = istring(minitl::format<256u>("Segment<%s>") | be_typeid<T>::name());
    return s_result;
}


template< typename T >
BE_EXPORT
raw<const RTTI::Class> be_typeid< KernelScheduler::Segment<T> >::klass()
{
    static const RTTI::Class s_class = {
        name(),
        u32(sizeof(KernelScheduler::Segment<T>)),
        0,
        RTTI::ClassType_Object,
        {0},
        {be_typeid< KernelScheduler::IParameter >::klass().m_ptr},
        {0},
        { 0 },
        { 0, 0 },
        { 0, 0 },
        { 0 },
        { 0 },
        0,
        0
    };
    raw< const RTTI::Class > result = { &s_class };
    return result;
}

template< typename T >
BE_EXPORT
RTTI::Type be_typeid<  KernelScheduler::Segment<T> >::type()
{
    return RTTI::Type::makeType(klass(), RTTI::Type::Value,
                                RTTI::Type::Mutable, RTTI::Type::Mutable);
}

}

/**************************************************************************************************/
#endif
