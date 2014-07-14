/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <text/stdafx.h>
#include    <outlinefontmanager.hh>
#include    <text/outlinefont.script.hh>
#include    <resource/resourcemanager.hh>
#include    <freetypelib.hh>
#include    <freetypeface.hh>

namespace BugEngine
{

OutlineFontManager::OutlineFontManager(weak<Resource::ResourceManager> manager,
                                       weak<FreetypeLibrary> freetype,
                                       weak<const FontList> fontList)
    :   m_manager(manager)
    ,   m_freetype(freetype)
    ,   m_fontList(fontList)
{
}

OutlineFontManager::~OutlineFontManager()
{
}

void OutlineFontManager::load(weak<const Resource::Description> description,
                              Resource::Resource& /*resource*/)
{
    be_info("loading outline font");
    if (be_checked_cast<const OutlineFont>(description)->m_fontFile)
    {
        m_manager->addTicket(this, description,
                             be_checked_cast<const OutlineFont>(description)->m_fontFile,
                             FileBinary, LoadFirstTime);
    }
    else
    {
        /* load from system font */
        be_unimplemented();
    }
}

void OutlineFontManager::reload(weak<const Resource::Description> /*oldDescription*/,
                                weak<const Resource::Description> newDescription,
                                Resource::Resource& /*resource*/)
{
    be_info("reloading outline font");
    if (be_checked_cast<const OutlineFont>(newDescription)->m_fontFile)
    {
        m_manager->addTicket(this, newDescription,
                             be_checked_cast<const OutlineFont>(newDescription)->m_fontFile,
                             FileBinary, LoadReload);
    }
    else
    {
        /* load from system font */
        be_unimplemented();
    }
}

void OutlineFontManager::unload(Resource::Resource& resource)
{
    be_info("unloading outline font");
    resource.clearRefHandle();
}

void OutlineFontManager::onTicketLoaded(weak<const Resource::Description> /*description*/,
                                        Resource::Resource& resource,
                                        const minitl::Allocator::Block<u8>& buffer,
                                        LoadType /*type*/)
{
    be_info("outline font file done loading");
    ref<FreetypeFace> face = ref<FreetypeFace>::create(Arena::game(), m_freetype, buffer);
    resource.clearRefHandle();
    resource.setRefHandle(face);
}

}
