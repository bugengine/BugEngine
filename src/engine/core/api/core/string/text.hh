/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_CORE_STRING_TEXT_HH_
#define BE_CORE_STRING_TEXT_HH_
/**************************************************************************************************/
#include    <core/stdafx.h>
#include    <minitl/format.hh>

namespace BugEngine
{

struct be_api(CORE) text
{
private:
    const char* m_text;
    const u32   m_length;
public:
    text(const char* str);
    text(const char* begin, const char* end);
    ~text();

    const char* begin() const;
    const char* end() const;

    u32 length() const;
    u32 size() const;

private:
    text();
};

}

namespace minitl {

template< u16 SIZE >
const format<SIZE>& operator|(const format<SIZE>& f, const BugEngine::text& value)
{
    return f|value.begin();
}

}


/**************************************************************************************************/
#endif
