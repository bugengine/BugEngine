/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <text/stdafx.h>
#include    <text/outlinefont.script.hh>

namespace BugEngine
{

OutlineFont::OutlineFont(const istring& fontname)
    :   m_systemFontName(fontname)
    ,   m_fontFile()
{
}

OutlineFont::OutlineFont(weak<const File> file)
    :   m_systemFontName("")
    ,   m_fontFile(file)
{
}

OutlineFont::~OutlineFont()
{
}

}

