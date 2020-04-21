/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_PARAMETER_IMAGE2D_FACTORY_HH_
#define BE_SCHEDULER_KERNEL_PARAMETER_IMAGE2D_FACTORY_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <rtti/typeinfo.hh>
#include    <rtti/classinfo.script.hh>

namespace BugEngine
{

namespace KernelScheduler
{
class IParameter;
template< typename T >
class Image2D;
}

namespace RTTI
{

template< typename T >
struct ClassID<  KernelScheduler::Image2D<T> >
{
    static BE_EXPORT raw<const RTTI::Class> klass()
    {
        static const RTTI::Class s_class = {
            istring(minitl::format<256u>("Image2D<%s>") | be_type<T>().name()),
            u32(sizeof(KernelScheduler::Image2D<T>)),
            0,
            RTTI::ClassType_Object,
            {0},
            {be_class< KernelScheduler::IParameter >().m_ptr},
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
};

}}

/**************************************************************************************************/
#endif
