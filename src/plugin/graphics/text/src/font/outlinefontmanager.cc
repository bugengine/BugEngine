/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <text/stdafx.h>
#include    <outlinefontmanager.hh>

namespace BugEngine
{

OutlineFontManager::OutlineFontManager(weak<const FontList> fontList)
    :   m_fontList(fontList)
{
}

OutlineFontManager::~OutlineFontManager()
{
}

void OutlineFontManager::load(weak<const Resource::Description> /*description*/,
                              Resource::Resource& /*resource*/)
{
    be_info("loading outline font");
}

void OutlineFontManager::reload(weak<const Resource::Description> /*oldDescription*/,
                                weak<const Resource::Description> /*newDescription*/,
                                Resource::Resource& /*resource*/)
{
    be_info("reloading outline font");
}

void OutlineFontManager::unload(Resource::Resource& /*resource*/)
{
    be_info("unloading outline font");
}

void OutlineFontManager::onTicketLoaded(weak<const Resource::Description> /*description*/,
                                        Resource::Resource& /*resource*/,
                                        const minitl::Allocator::Block<u8>& /*buffer*/,
                                        LoadType /*type*/)
{
    be_info("outline font file done loading");
}

}

