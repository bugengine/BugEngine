/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_STRING_TEXT_HH_
#define BE_CORE_STRING_TEXT_HH_
/**************************************************************************************************/
#include <bugengine/core/stdafx.h>
#include <bugengine/minitl/format.hh>

namespace BugEngine {

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

}  // namespace BugEngine

namespace minitl {

template < u16 SIZE >
const format< SIZE >& operator|(const format< SIZE >& f, const BugEngine::text& value)
{
    return f | value.begin();
}

}  // namespace minitl

/**************************************************************************************************/
#endif
