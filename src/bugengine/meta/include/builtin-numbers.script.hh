/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_BUILTIN_SCRIPT_HH_
#define BE_META_BUILTIN_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/builtin.hh>
#include <bugengine/meta/classinfo.script.hh>

#if 0

be_tag(Index(BugEngine::Meta::ClassType_Number
          + (BugEngine::Meta::ClassIndex_bool << 16)))
be_pod bool
{
};

be_tag(Index(BugEngine::Meta::ClassType_Number
          + (BugEngine::Meta::ClassIndex_u8 << 16)))
be_pod u8
{
};

be_tag(Index(BugEngine::Meta::ClassType_Number
          + (BugEngine::Meta::ClassIndex_u16 << 16)))
be_pod u16
{
};

be_tag(Index(BugEngine::Meta::ClassType_Number
          + (BugEngine::Meta::ClassIndex_u32 << 16)))
be_pod u32
{
};

be_tag(Index(BugEngine::Meta::ClassType_Number
          + (BugEngine::Meta::ClassIndex_u64 << 16)))
be_pod u64
{
};


be_tag(Index(BugEngine::Meta::ClassType_Number
          + (BugEngine::Meta::ClassIndex_i8 << 16)))
be_pod i8
{
};

be_tag(Index(BugEngine::Meta::ClassType_Number
          + (BugEngine::Meta::ClassIndex_i16 << 16)))
be_pod i16
{
};

be_tag(Index(BugEngine::Meta::ClassType_Number
          + (BugEngine::Meta::ClassIndex_i32 << 16)))
be_pod i32
{
};

be_tag(Index(BugEngine::Meta::ClassType_Number
          + (BugEngine::Meta::ClassIndex_i64 << 16)))
be_pod i64
{
};

be_tag(Index(BugEngine::Meta::ClassType_Number
          + (BugEngine::Meta::ClassIndex_float << 16)))
be_pod float
{
};

be_tag(Index(BugEngine::Meta::ClassType_Number
          + (BugEngine::Meta::ClassIndex_double << 16)))
be_pod double
{
};

#endif

/**************************************************************************************************/
#endif
