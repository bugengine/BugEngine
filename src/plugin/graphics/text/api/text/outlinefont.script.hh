/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_TEXT_OUTLINEFONT_SCRIPT_HH_
#define BE_TEXT_OUTLINEFONT_SCRIPT_HH_
/*****************************************************************************/
#include    <filesystem/file.script.hh>
#include    <resource/description.script.hh>

namespace BugEngine
{

class OutlineFontManager;

class be_api(TEXT) OutlineFont : public Resource::Description
{
    friend class OutlineFontManager;
private:
    istring const           m_systemFontName;
    weak<const File> const  m_fontFile;
public:
    OutlineFont(const istring& fontname);
    OutlineFont(weak<const File> file);
    ~OutlineFont();
};

}

/*****************************************************************************/
#endif
