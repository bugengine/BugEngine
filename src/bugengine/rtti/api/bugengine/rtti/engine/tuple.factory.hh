/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_TUPLE_HH_
#define BE_RTTI_ENGINE_TUPLE_HH_
/**************************************************************************************************/
#include <bugengine/rtti/stdafx.h>
#include <bugengine/minitl/tuple.hh>
#include <bugengine/rtti/classinfo.script.hh>
#include <bugengine/rtti/engine/helper/method.hh>
#include <bugengine/rtti/engine/methodinfo.script.hh>
#include <bugengine/rtti/engine/objectinfo.script.hh>
#include <bugengine/rtti/typeinfo.hh>

namespace BugEngine {

template < typename T1, typename T2, typename T3, typename T4, typename T5 >
static RTTI::Value make_tuple(RTTI::Value* v, u32 count);

}

#include <bugengine/rtti/engine/helper/tuple2.factory.hh>
#include <bugengine/rtti/engine/helper/tuple3.factory.hh>
#include <bugengine/rtti/engine/helper/tuple4.factory.hh>
#include <bugengine/rtti/engine/helper/tuple5.factory.hh>

/**************************************************************************************************/
#endif
