/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_BUILTIN_SCRIPT_HH_
#define BE_RTTI_BUILTIN_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/rtti/stdafx.h>
#include <bugengine/rtti/builtin.hh>
#include <bugengine/rtti/classinfo.script.hh>

#if 0

namespace BugEngine
{

be_tag(Index(BugEngine::RTTI::ClassType_String
          + (BugEngine::RTTI::ClassIndex_istring << 16)))
struct istring
{
};

be_tag(Index(BugEngine::RTTI::ClassType_String
          + (BugEngine::RTTI::ClassIndex_inamespace << 16)))
struct inamespace
{
published:
    u32 size() const;
    const istring& operator[](u32 index) const;

    void push_back(const istring& component);
    istring pop_back();
    istring pop_front();
};

be_tag(Index(BugEngine::RTTI::ClassType_String
          + (BugEngine::RTTI::ClassIndex_ifilename << 16)))
struct ifilename
{
published:
    u32 size() const;
    const istring& operator[](u32 index) const;

    void push_back(const istring& component);
    istring pop_back();
    istring pop_front();
};

be_tag(Index(BugEngine::RTTI::ClassType_String
          + (BugEngine::RTTI::ClassIndex_ipath << 16)))
struct ipath
{
published:
    u32 size() const;
    const istring& operator[](u32 index) const;

    void push_back(const istring& component);
    istring pop_back();
    istring pop_front();
};

be_tag(Index(BugEngine::RTTI::ClassType_String
          + (BugEngine::RTTI::ClassIndex_text << 16)))
struct text
{
published:
    u32 length() const;
};

}

#endif

/**************************************************************************************************/
#endif
