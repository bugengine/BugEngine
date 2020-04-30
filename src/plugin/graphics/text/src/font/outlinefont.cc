/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.graphics.text/stdafx.h>
#include <bugengine/plugin.graphics.text/outlinefont.script.hh>

namespace BugEngine {

OutlineFont::OutlineFont(const istring& fontSystemName)
    : m_fontSystemName(fontSystemName)
    , m_fontFile()
{
}

OutlineFont::OutlineFont(weak< const File > fontFile) : m_fontSystemName(""), m_fontFile(fontFile)
{
}

OutlineFont::~OutlineFont()
{
}

}  // namespace BugEngine
