/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_TEXT_OUTLINEFONTMANAGER_HH_
#define BE_TEXT_OUTLINEFONTMANAGER_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.text/stdafx.h>
#include <bugengine/resource/loader.hh>

namespace BugEngine {

class FreetypeLibrary;
class FontList;

class OutlineFontManager : public Resource::ILoader
{
private:
    weak< Resource::ResourceManager > m_manager;
    weak< FreetypeLibrary >           m_freetype;
    weak< const FontList >            m_fontList;

public:
    OutlineFontManager(weak< Resource::ResourceManager > manager, weak< FreetypeLibrary > freetype,
                       weak< const FontList > fontList);
    ~OutlineFontManager();

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
