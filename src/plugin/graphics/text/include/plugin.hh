/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_TEXT_TEXTPLUGIN_HH_
#define BE_TEXT_TEXTPLUGIN_HH_
/*****************************************************************************/
#include    <minitl/pointer.hh>
#include    <plugin/plugin.hh>

namespace BugEngine
{

class TextManager;
class OutlineFontManager;
class BitmapFontManager;
class FontList;
class FreetypeLibrary;

class TextPlugin : public minitl::pointer
{
private:
    weak<Resource::ResourceManager> m_resourceManager;
    scoped<FreetypeLibrary>         m_freetypeLibrary;
    scoped<FontList>                m_fontList;
    scoped<TextManager>             m_textManager;
    scoped<OutlineFontManager>      m_outlineFontManager;
    scoped<BitmapFontManager>       m_bitmapFontManager;
public:
    TextPlugin(const Plugin::Context& pluginContext);
    ~TextPlugin();
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); ::operator delete(memory); }
};

}

/*****************************************************************************/
#endif
