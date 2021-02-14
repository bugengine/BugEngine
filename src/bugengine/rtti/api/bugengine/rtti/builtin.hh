/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_BUILTIN_HH_
#define BE_RTTI_BUILTIN_HH_
/**************************************************************************************************/
#include <bugengine/rtti/stdafx.h>
#include <bugengine/rtti/classinfo.script.hh>

namespace BugEngine { namespace RTTI {

struct Type;

enum ClassIndex_Numeric
{
    ClassIndex_bool   = 0,
    ClassIndex_u8     = 1,
    ClassIndex_u16    = 2,
    ClassIndex_u32    = 3,
    ClassIndex_u64    = 4,
    ClassIndex_i8     = 5,
    ClassIndex_i16    = 6,
    ClassIndex_i32    = 7,
    ClassIndex_i64    = 8,
    ClassIndex_float  = 9,
    ClassIndex_double = 10
};

enum ClassIndex_String
{
    ClassIndex_istring    = 0,
    ClassIndex_inamespace = 1,
    ClassIndex_ifilename  = 2,
    ClassIndex_ipath      = 3,
    ClassIndex_text       = 4
};

const Type& getTypeFromIndex(ClassIndex_Numeric index);
const Type& getTypeFromIndex(ClassIndex_String index);

}}  // namespace BugEngine::RTTI

/**************************************************************************************************/
#endif
