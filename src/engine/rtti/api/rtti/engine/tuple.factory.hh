/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_TUPLE_HH_
#define BE_RTTI_ENGINE_TUPLE_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/typeinfo.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/objectinfo.script.hh>
#include    <rtti/engine/helper/method.hh>
#include    <minitl/tuple.hh>

namespace BugEngine
{

template< typename T1, typename T2, typename T3, typename T4, typename T5 >
static RTTI::Value make_tuple(RTTI::Value* v, u32 count);

}

#include    <rtti/engine/helper/tuple2.factory.hh>
#include    <rtti/engine/helper/tuple3.factory.hh>
#include    <rtti/engine/helper/tuple4.factory.hh>
#include    <rtti/engine/helper/tuple5.factory.hh>

/**************************************************************************************************/
#endif

