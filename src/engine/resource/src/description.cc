/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <resource/stdafx.h>
#include    <resource/description.script.hh>
#include    <resource/loader.hh>

namespace BugEngine { namespace Resource
{

Description::Description()
{
}

Description::~Description()
{
}

void Description::load(weak<ILoader> loader) const
{
    for(int i = 0; i < MaxResourceCount; ++i)
    {
        if (!m_resources[i].m_handle.owner)
        {
            m_resources[i].m_handle.owner = loader->m_id;
            loader->load(this, m_resources[i]);
            return;
        }
    }
    be_notreached();
}

void Description::unload(weak<ILoader> loader) const
{
    for(int i = 0; i < MaxResourceCount; ++i)
    {
        if (m_resources[i].m_handle.owner == loader->m_id)
        {
            loader->unload(m_resources[i]);
            m_resources[i].m_handle.owner = 0;
            m_resources[i].m_handle.id.ptrId = 0;
            return;
        }
    }
    be_notreached();
}

Resource& Description::getResourceForWriting(weak<const ILoader> owner) const
{
    for(int i = 0; i < MaxResourceCount; ++i)
    {
        if (m_resources[i].m_handle.owner == owner->m_id)
        {
            return m_resources[i];
        }
    }
    return Resource::null();
}

const Resource& Description::getResource(weak<const ILoader> owner) const
{
    for(int i = 0; i < MaxResourceCount; ++i)
    {
        if (m_resources[i].m_handle.owner == owner->m_id)
        {
            return m_resources[i];
        }
    }
    return Resource::null();
}

}}
