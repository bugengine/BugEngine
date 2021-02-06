/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_TEXT_OUTLINEFONT_SCRIPT_HH_
#define BE_TEXT_OUTLINEFONT_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.text/stdafx.h>
#include <bugengine/filesystem/file.script.hh>
#include <bugengine/resource/description.script.hh>

namespace BugEngine {

class OutlineFontManager;

class be_api(TEXT) OutlineFont : public Resource::Description
{
    friend class OutlineFontManager;

private:
    istring const            m_fontSystemName;
    weak< const File > const m_fontFile;
published:
    OutlineFont(const istring& fontSystemName);
    OutlineFont(weak< const File > fontFile);
    ~OutlineFont();
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
