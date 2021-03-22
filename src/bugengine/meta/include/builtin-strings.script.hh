/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_BUILTIN_SCRIPT_HH_
#define BE_META_BUILTIN_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/builtin.hh>
#include <bugengine/meta/classinfo.script.hh>

#if 0

namespace BugEngine
{

be_tag(Index(BugEngine::Meta::ClassType_String
          + (BugEngine::Meta::ClassIndex_istring << 16)))
struct istring
{
};

be_tag(Index(BugEngine::Meta::ClassType_String
          + (BugEngine::Meta::ClassIndex_inamespace << 16)))
struct inamespace
{
published:
    u32 size() const;
    const istring& operator[](u32 index) const;

    void push_back(const istring& component);
    istring pop_back();
    istring pop_front();
};

be_tag(Index(BugEngine::Meta::ClassType_String
          + (BugEngine::Meta::ClassIndex_ifilename << 16)))
struct ifilename
{
published:
    u32 size() const;
    const istring& operator[](u32 index) const;

    void push_back(const istring& component);
    istring pop_back();
    istring pop_front();
};

be_tag(Index(BugEngine::Meta::ClassType_String
          + (BugEngine::Meta::ClassIndex_ipath << 16)))
struct ipath
{
published:
    u32 size() const;
    const istring& operator[](u32 index) const;

    void push_back(const istring& component);
    istring pop_back();
    istring pop_front();
};

be_tag(Index(BugEngine::Meta::ClassType_String
          + (BugEngine::Meta::ClassIndex_text << 16)))
struct text
{
published:
    u32 length() const;
};

}

#endif

/**************************************************************************************************/
#endif
