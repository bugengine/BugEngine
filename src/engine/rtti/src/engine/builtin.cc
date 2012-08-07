/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/helper/get.hh>
#include    <rtti/engine/helper/set.hh>
#include    <rtti/engine/helper/method.hh>


BE_REGISTER_NAMESPACE_2_(game, BugEngine, RTTI);
BE_REGISTER_NAMESPACE_2_(game, BugEngine, EditHint);
BE_REGISTER_NAMESPACE_2_(game, BugEngine, Documentation);
BE_REGISTER_NAMESPACE_2_(game, BugEngine, Kernel);

namespace BugEngine
{

template< > BE_EXPORT raw<const RTTI::Class> be_typeid< void >::klass()
{
    static const RTTI::Class s_void = { "void", {0}, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, &RTTI::nullconstructor<0>, &RTTI::nulldestructor };
    raw<const RTTI::Class> ci = {&s_void};
    return ci;
}

template< >
BE_EXPORT raw<const RTTI::Class> be_typeid< minitl::pointer >::klass()
{
    static const RTTI::Class s_pointer = { "pointer", {0}, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, &RTTI::nullconstructor<0>, &RTTI::nulldestructor };
    raw<const RTTI::Class> ci = {&s_pointer};
    return ci;
}

template< >
BE_EXPORT raw<const RTTI::Class> be_typeid< minitl::refcountable >::klass()
{
    static const RTTI::Class s_refcountable = { "refcountable", {0}, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, &RTTI::nullconstructor<0>, &RTTI::nulldestructor };
    raw<const RTTI::Class> ci = {&s_refcountable};
    return ci;
}

}
