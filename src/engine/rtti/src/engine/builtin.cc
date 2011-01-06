/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/typeinfo.hh>
#include    <rtti/engine/classinfo.script.hh>
#include    <rtti/engine/wrapper.hh>

namespace BugEngine
{

template< > ref<const RTTI::ClassInfo> const be_typeid< void >::klass()
{
    static ref<const RTTI::ClassInfo> klass = ref<RTTI::ClassInfo>::create(rttiArena(), inamespace("void"), ref<RTTI::ClassInfo>(), ref<RTTI::ClassInfo>(), sizeof(minitl::refcountable));
    return klass;
}

template< > ref<const RTTI::ClassInfo> const be_typeid< ::minitl::refcountable >::klass()
{
    static ref<const RTTI::ClassInfo> klass = ref<RTTI::ClassInfo>::create(rttiArena(), inamespace("minitl.refcountable"), be_typeid< void >::klass(), ref<RTTI::ClassInfo>(), sizeof(minitl::refcountable));
    return klass;
}

}
