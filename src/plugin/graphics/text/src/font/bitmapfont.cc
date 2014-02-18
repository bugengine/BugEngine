/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <text/stdafx.h>
#include    <text/bitmapfont.script.hh>

namespace BugEngine
{

BitmapFont::BitmapFont(const istring& fontname)
    :   m_systemFontName(fontname)
    ,   m_fontFile()
{
}

BitmapFont::BitmapFont(weak<const File> file)
    :   m_systemFontName("")
    ,   m_fontFile(file)
{
}

BitmapFont::~BitmapFont()
{
}

}

