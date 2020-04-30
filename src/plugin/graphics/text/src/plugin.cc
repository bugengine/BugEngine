/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.graphics.text/stdafx.h>
#include <bugengine/plugin.graphics.text/bitmapfont.script.hh>
#include <bugengine/plugin.graphics.text/outlinefont.script.hh>
#include <bugengine/plugin.graphics.text/text.script.hh>
#include <bugengine/plugin/plugin.hh>
#include <bugengine/resource/resourcemanager.hh>
#include <bitmapfontmanager.hh>
#include <fontlist.hh>
#include <freetypelib.hh>
#include <outlinefontmanager.hh>
#include <plugin.hh>
#include <textmanager.hh>

namespace BugEngine {

TextPlugin::TextPlugin(const Plugin::Context& pluginContext)
    : m_resourceManager(pluginContext.resourceManager)
    , m_freetypeLibrary(scoped< FreetypeLibrary >::create(Arena::game()))
    , m_fontList(scoped< FontList >::create(Arena::game()))
    , m_textManager(scoped< TextManager >::create(Arena::game()))
    , m_outlineFontManager(scoped< OutlineFontManager >::create(Arena::game(), m_resourceManager,
                                                                m_freetypeLibrary, m_fontList))
    , m_bitmapFontManager(scoped< BitmapFontManager >::create(Arena::game(), m_resourceManager,
                                                              m_freetypeLibrary, m_fontList))
{
    m_resourceManager->attach(be_class< Text >(), m_textManager);
    m_resourceManager->attach(be_class< OutlineFont >(), m_outlineFontManager);
    m_resourceManager->attach(be_class< BitmapFont >(), m_bitmapFontManager);
}

TextPlugin::~TextPlugin()
{
    m_resourceManager->detach(be_class< BitmapFont >(), m_bitmapFontManager);
    m_resourceManager->detach(be_class< OutlineFont >(), m_outlineFontManager);
    m_resourceManager->detach(be_class< Text >(), m_textManager);
}

}  // namespace BugEngine

BE_PLUGIN_REGISTER(BugEngine::TextPlugin);
