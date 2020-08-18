/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/rtti/stdafx.h>
#include <bugengine/rtti/classinfo.script.hh>
#include <bugengine/rtti/engine/array.factory.hh>
#include <bugengine/rtti/engine/helper/method.hh>
#include <bugengine/rtti/engine/namespace.hh>

namespace BugEngine { namespace RTTI {

template <>
BE_EXPORT raw< const RTTI::Class > ClassID< void >::klass()
{
    static RTTI::Class s_void = {"void",
                                 0,
                                 0,
                                 0,
                                 {0},
                                 {0},
                                 {0},
                                 {0},
                                 {0, 0},
                                 {0, 0},
                                 {0},
                                 {0},
                                 &RTTI::nullconstructor< 0 >,
                                 &RTTI::nulldestructor};
    raw< RTTI::Class > ci     = {&s_void};
    return ci;
}

template <>
BE_EXPORT raw< const RTTI::Class > ClassID< minitl::pointer >::klass()
{
    static RTTI::Class s_pointer
       = {"pointer", 0,      0,      0,   {0}, {be_class< void >().m_ptr},  {0},
          {0},       {0, 0}, {0, 0}, {0}, {0}, &RTTI::nullconstructor< 0 >, &RTTI::nulldestructor};
    raw< RTTI::Class > ci = {&s_pointer};
    return ci;
}

template <>
BE_EXPORT raw< const RTTI::Class > ClassID< minitl::refcountable >::klass()
{
    static RTTI::Class s_refcountable = {"refcountable",
                                         0,
                                         0,
                                         0,
                                         {0},
                                         {be_class< minitl::pointer >().m_ptr},
                                         {0},
                                         {0},
                                         {0, 0},
                                         {0, 0},
                                         {0},
                                         {0},
                                         &RTTI::nullconstructor< 0 >,
                                         &RTTI::nulldestructor};
    raw< RTTI::Class > ci             = {&s_refcountable};
    return ci;
}

}}  // namespace BugEngine::RTTI
