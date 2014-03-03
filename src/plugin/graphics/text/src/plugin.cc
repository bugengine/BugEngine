/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <text/stdafx.h>
#include    <plugin/plugin.hh>
#include    <resource/resourcemanager.hh>
#include    <plugin.hh>
#include    <fontlist.hh>
#include    <text/text.script.hh>
#include    <text/outlinefont.script.hh>
#include    <text/bitmapfont.script.hh>
#include    <freetypelib.hh>
#include    <textmanager.hh>
#include    <outlinefontmanager.hh>
#include    <bitmapfontmanager.hh>

namespace BugEngine
{

TextPlugin::TextPlugin(const Plugin::Context& pluginContext)
    :   m_resourceManager(pluginContext.resourceManager)
    ,   m_freetypeLibrary(scoped<FreetypeLibrary>::create(Arena::game()))
    ,   m_fontList(scoped<FontList>::create(Arena::game()))
    ,   m_textManager(scoped<TextManager>::create(Arena::game()))
    ,   m_outlineFontManager(scoped<OutlineFontManager>::create(Arena::game(), m_resourceManager, m_freetypeLibrary, m_fontList))
    ,   m_bitmapFontManager(scoped<BitmapFontManager>::create(Arena::game(), m_resourceManager, m_freetypeLibrary, m_fontList))
{
    m_resourceManager->attach(be_typeid<Text>::klass(), m_textManager);
    m_resourceManager->attach(be_typeid<OutlineFont>::klass(), m_outlineFontManager);
    m_resourceManager->attach(be_typeid<BitmapFont>::klass(), m_bitmapFontManager);
}

TextPlugin::~TextPlugin()
{
    m_resourceManager->detach(be_typeid<BitmapFont>::klass(), m_bitmapFontManager);
    m_resourceManager->detach(be_typeid<OutlineFont>::klass(), m_outlineFontManager);
    m_resourceManager->detach(be_typeid<Text>::klass(), m_textManager);
}

}

BE_PLUGIN_REGISTER(BugEngine::TextPlugin);
