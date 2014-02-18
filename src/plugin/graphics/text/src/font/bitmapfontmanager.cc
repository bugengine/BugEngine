/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <text/stdafx.h>
#include    <bitmapfontmanager.hh>
#include    <fontlist.hh>

namespace BugEngine
{

BitmapFontManager::BitmapFontManager(weak<const FontList> fontList)
    :   m_fontList(fontList)
{
}

BitmapFontManager::~BitmapFontManager()
{
}

void BitmapFontManager::load(weak<const Resource::Description> /*description*/, Resource::Resource& /*resource*/)
{
    be_info("loading bitmap font");
}

void BitmapFontManager::reload(weak<const Resource::Description> /*oldDescription*/, weak<const Resource::Description> /*newDescription*/, Resource::Resource& /*resource*/)
{
    be_info("reloading bitmap font");
}

void BitmapFontManager::unload(Resource::Resource& /*resource*/)
{
    be_info("unloading bitmap font");
}

void BitmapFontManager::onTicketLoaded(weak<const Resource::Description> /*description*/,
                                       Resource::Resource& /*resource*/,
                                       const minitl::Allocator::Block<u8>& /*buffer*/,
                                       LoadType /*type*/)
{
    be_info("bitmap font file done loading");
}

}

