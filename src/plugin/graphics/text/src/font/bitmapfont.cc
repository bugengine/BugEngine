/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.graphics.text/stdafx.h>
#include <bugengine/plugin.graphics.3d/shader/types.script.hh>
#include <bugengine/plugin.graphics.text/bitmapfont.script.hh>

namespace BugEngine {

BitmapFont::BitmapFont(const istring& fontSystemName)
    : m_fontSystemName(fontSystemName)
    , m_fontFile()
{
}

BitmapFont::BitmapFont(weak< const File > fontFile) : m_fontSystemName(""), m_fontFile(fontFile)
{
}

BitmapFont::~BitmapFont()
{
}

}  // namespace BugEngine
