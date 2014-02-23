/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <text/stdafx.h>
#include    <text/bitmapfont.script.hh>
#include    <3d/shader/types.script.hh>

namespace BugEngine
{

BitmapFont::BitmapFont(const istring& fontSystemName)
    :   m_fontSystemName(fontSystemName)
    ,   m_fontFile()
{
}

BitmapFont::BitmapFont(weak<const File> fontFile)
    :   m_fontSystemName("")
    ,   m_fontFile(fontFile)
{
}

BitmapFont::~BitmapFont()
{
}

}

