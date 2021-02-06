/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.graphics.text/stdafx.h>
#include <bitmapfontmanager.hh>
#include <fontlist.hh>

namespace BugEngine {

BitmapFontManager::BitmapFontManager(weak< Resource::ResourceManager > manager,
                                     weak< FreetypeLibrary >           freetype,
                                     weak< const FontList >            fontList)
    : m_manager(manager)
    , m_freetype(freetype)
    , m_fontList(fontList)
{
}

BitmapFontManager::~BitmapFontManager()
{
}

void BitmapFontManager::load(weak< const Resource::Description > /*description*/,
                             Resource::Resource& /*resource*/)
{
    be_info("loading bitmap font");
}

void BitmapFontManager::reload(weak< const Resource::Description > /*oldDescription*/,
                               weak< const Resource::Description > /*newDescription*/,
                               Resource::Resource& /*resource*/)
{
    be_info("reloading bitmap font");
}

void BitmapFontManager::unload(Resource::Resource& /*resource*/)
{
    be_info("unloading bitmap font");
}

void BitmapFontManager::onTicketLoaded(weak< const Resource::Description > /*description*/,
                                       Resource::Resource& /*resource*/,
                                       const minitl::Allocator::Block< u8 >& /*buffer*/,
                                       LoadType /*type*/)
{
    be_info("bitmap font file done loading");
}

}  // namespace BugEngine
