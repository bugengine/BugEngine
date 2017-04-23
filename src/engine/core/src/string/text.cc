/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/string/text.hh>
#include    <cstring>


namespace BugEngine
{

text::text(const char* str)
    :   m_text(Arena::general().strdup(str))
    ,   m_length(be_checked_numcast<u32>(strlen(str)))
{
}

text::text(const char *begin, const char *end)
    :   m_text(Arena::general().strdup(begin, end))
    ,   m_length(be_checked_numcast<u32>(end - begin))
{
}

text::~text()
{
    Arena::general().free(m_text);
}

const char* text::begin() const
{
    return m_text;
}

const char* text::end() const
{
    return m_text + m_length;
}

u32 text::length() const
{
    return m_length;
}

u32 text::size() const
{
    return m_length;
}

}
