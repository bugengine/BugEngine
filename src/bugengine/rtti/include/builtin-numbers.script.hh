/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_BUILTIN_SCRIPT_HH_
#define BE_RTTI_BUILTIN_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/rtti/stdafx.h>
#include <bugengine/rtti/builtin.hh>
#include <bugengine/rtti/classinfo.script.hh>

#if 0

be_tag(Index(BugEngine::RTTI::ClassType_Number
          + (BugEngine::RTTI::ClassIndex_bool << 16)))
be_pod bool
{
};

be_tag(Index(BugEngine::RTTI::ClassType_Number
          + (BugEngine::RTTI::ClassIndex_u8 << 16)))
be_pod u8
{
};

be_tag(Index(BugEngine::RTTI::ClassType_Number
          + (BugEngine::RTTI::ClassIndex_u16 << 16)))
be_pod u16
{
};

be_tag(Index(BugEngine::RTTI::ClassType_Number
          + (BugEngine::RTTI::ClassIndex_u32 << 16)))
be_pod u32
{
};

be_tag(Index(BugEngine::RTTI::ClassType_Number
          + (BugEngine::RTTI::ClassIndex_u64 << 16)))
be_pod u64
{
};


be_tag(Index(BugEngine::RTTI::ClassType_Number
          + (BugEngine::RTTI::ClassIndex_i8 << 16)))
be_pod i8
{
};

be_tag(Index(BugEngine::RTTI::ClassType_Number
          + (BugEngine::RTTI::ClassIndex_i16 << 16)))
be_pod i16
{
};

be_tag(Index(BugEngine::RTTI::ClassType_Number
          + (BugEngine::RTTI::ClassIndex_i32 << 16)))
be_pod i32
{
};

be_tag(Index(BugEngine::RTTI::ClassType_Number
          + (BugEngine::RTTI::ClassIndex_i64 << 16)))
be_pod i64
{
};

be_tag(Index(BugEngine::RTTI::ClassType_Number
          + (BugEngine::RTTI::ClassIndex_float << 16)))
be_pod float
{
};

be_tag(Index(BugEngine::RTTI::ClassType_Number
          + (BugEngine::RTTI::ClassIndex_double << 16)))
be_pod double
{
};

#endif

/**************************************************************************************************/
#endif
