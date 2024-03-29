/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_TEXT_TEXTPLUGIN_HH_
#define BE_TEXT_TEXTPLUGIN_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.text/stdafx.h>
#include <bugengine/minitl/pointer.hh>
#include <bugengine/plugin/plugin.hh>

namespace BugEngine {

class TextManager;
class OutlineFontManager;
class BitmapFontManager;
class FontList;
class FreetypeLibrary;

class TextPlugin : public minitl::refcountable
{
private:
    weak< Resource::ResourceManager > m_resourceManager;
    scoped< FreetypeLibrary >         m_freetypeLibrary;
    scoped< FontList >                m_fontList;
    scoped< TextManager >             m_textManager;
    scoped< OutlineFontManager >      m_outlineFontManager;
    scoped< BitmapFontManager >       m_bitmapFontManager;

public:
    TextPlugin(const Plugin::Context& pluginContext);
    ~TextPlugin();
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
