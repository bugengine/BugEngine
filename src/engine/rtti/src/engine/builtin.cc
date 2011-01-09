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
    static ref<const RTTI::ClassInfo> klass = ref<RTTI::ClassInfo>::create(rttiArena(), inamespace("void"), ref<RTTI::ClassInfo>(), ref<RTTI::ClassInfo>(), 0, 0);
    return klass;
}

template< > ref<const RTTI::ClassInfo> const be_typeid< ::minitl::refcountable >::klass()
{
    return  be_typeid< void >::klass();
}

}
