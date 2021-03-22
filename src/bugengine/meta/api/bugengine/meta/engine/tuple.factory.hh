/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_ENGINE_TUPLE_HH_
#define BE_META_ENGINE_TUPLE_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/classinfo.script.hh>
#include <bugengine/meta/engine/helper/method.hh>
#include <bugengine/meta/engine/methodinfo.script.hh>
#include <bugengine/meta/engine/objectinfo.script.hh>
#include <bugengine/meta/typeinfo.hh>
#include <bugengine/minitl/tuple.hh>

namespace BugEngine {

template < typename T1, typename T2, typename T3, typename T4, typename T5 >
static Meta::Value make_tuple(Meta::Value* v, u32 count);

}

#include <bugengine/meta/engine/helper/tuple2.factory.hh>
#include <bugengine/meta/engine/helper/tuple3.factory.hh>
#include <bugengine/meta/engine/helper/tuple4.factory.hh>
#include <bugengine/meta/engine/helper/tuple5.factory.hh>

/**************************************************************************************************/
#endif
