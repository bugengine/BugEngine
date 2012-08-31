/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_RESOURCE_ILOADER_HH_
#define BE_RESOURCE_ILOADER_HH_
/*****************************************************************************/
#include    <resource/resource.hh>
#include    <resource/description.script.hh>

namespace BugEngine { namespace Resource
{

class ResourceManager;

class be_api(RESOURCE) ILoader : public minitl::refcountable
{
    friend class ResourceManager;
    friend class Description;
    BE_NOCOPY(ILoader);
protected:
    const u32   m_id;
protected:
    ILoader();
    ~ILoader();

    virtual void load(weak<const Description> description, Resource& resource) = 0;
    virtual void unload(Resource& resource) = 0;

    virtual void onTicketUpdated(weak<const Description> /*description*/, Resource& /*resource*/, const minitl::Allocator::Block<u8>& /*buffer*/, u32 /*progress*/) { }
    virtual void onTicketLoaded(weak<const Description> /*description*/, Resource& /*resource*/, const minitl::Allocator::Block<u8>& /*buffer*/) { be_notreached(); }
};

}}

/*****************************************************************************/
#endif
