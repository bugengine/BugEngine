/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_TEXT_BITMAPFONTMANAGER_HH_
#define BE_TEXT_BITMAPFONTMANAGER_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.text/stdafx.h>
#include <bugengine/resource/loader.hh>
#include <bugengine/resource/resourcemanager.hh>
#include <freetypelib.hh>

namespace BugEngine {

class FreetypeLibrary;
class FontList;

class BitmapFontManager : public Resource::ILoader
{
private:
    weak< Resource::ResourceManager > m_manager;
    weak< FreetypeLibrary >           m_freetype;
    weak< const FontList >            m_fontList;

public:
    BitmapFontManager(weak< Resource::ResourceManager > manager, weak< FreetypeLibrary > freetype,
                      weak< const FontList > fontList);
    ~BitmapFontManager();

    void load(weak< const Resource::Description > description,
              Resource::Resource&                 resource) override;
    void reload(weak< const Resource::Description > oldDescription,
                weak< const Resource::Description > newDescription,
                Resource::Resource&                 resource) override;
    void unload(Resource::Resource& resource) override;
    void onTicketLoaded(weak< const Resource::Description > description,
                        Resource::Resource& resource, const minitl::Allocator::Block< u8 >& buffer,
                        LoadType type) override;
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
