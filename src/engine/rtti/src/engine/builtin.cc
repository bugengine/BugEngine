/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/namespace.hh>
#include    <rtti/engine/array.hh>
#include    <rtti/engine/helper/method.hh>

BE_REGISTER_NAMESPACE_2_(game, BugEngine, RTTI);
BE_REGISTER_NAMESPACE_2_(game, BugEngine, EditHint);
BE_REGISTER_NAMESPACE_2_(game, BugEngine, Documentation);

namespace BugEngine
{

template< > BE_EXPORT raw<RTTI::Class> be_typeid< void >::preklass()
{
    static RTTI::Class s_void = { "void", 0, 0, 0, { 0 },{ 0 }, { 0 },
                                  {&RTTI::staticarray<const RTTI::Tag>::s_null},
                                  {&RTTI::staticarray<const RTTI::Property>::s_null},
                                  {&RTTI::staticarray<const RTTI::Method>::s_null},
                                  {0}, &RTTI::nullconstructor<0>, &RTTI::nulldestructor };
    raw<RTTI::Class> ci = {&s_void};
    return ci;
}
template< > BE_EXPORT raw<const RTTI::Class> be_typeid< void >::registerProperties()
{
    return preklass();
}


template< > BE_EXPORT raw<RTTI::Class> be_typeid< minitl::pointer >::preklass()
{
    static RTTI::Class s_pointer = { "pointer", 0, 0, 0, { 0 },
                                     {be_typeid< void >::preklass().m_ptr}, { 0 },
                                     {&RTTI::staticarray<const RTTI::Tag>::s_null},
                                     {&RTTI::staticarray<const RTTI::Property>::s_null},
                                     {&RTTI::staticarray<const RTTI::Method>::s_null},
                                     {0}, &RTTI::nullconstructor<0>, &RTTI::nulldestructor };
    raw<RTTI::Class> ci = {&s_pointer};
    return ci;
}

template< > BE_EXPORT raw<const RTTI::Class> be_typeid< minitl::pointer >::registerProperties()
{
    return preklass();
}

template< > BE_EXPORT raw<RTTI::Class> be_typeid< minitl::refcountable >::preklass()
{
    static RTTI::Class s_refcountable = { "refcountable", 0, 0, 0, { 0 },
                                          {be_typeid< minitl::pointer >::preklass().m_ptr}, { 0 },
                                          {&RTTI::staticarray<const RTTI::Tag>::s_null},
                                          {&RTTI::staticarray<const RTTI::Property>::s_null},
                                          {&RTTI::staticarray<const RTTI::Method>::s_null},
                                          {0}, &RTTI::nullconstructor<0>, &RTTI::nulldestructor };
    raw<RTTI::Class> ci = {&s_refcountable};
    return ci;
}

template< > BE_EXPORT raw<const RTTI::Class> be_typeid< minitl::refcountable >::registerProperties()
{
    return preklass();
}

}
