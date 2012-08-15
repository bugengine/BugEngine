/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_RESOURCE_RESOURCE_SCRIPT_HH_
#define BE_RESOURCE_RESOURCE_SCRIPT_HH_
/*****************************************************************************/
#include    <minitl/refcountable.hh>
#include    <resource/resourcehandle.hh>
#include    <minitl/intrusive_list.hh>

namespace BugEngine
{

class IResourceLoader;
class ResourceManager;

class be_api(RESOURCE) Resource : public minitl::refcountable, public minitl::intrusive_list<const Resource, 2>::item
{
    friend class ResourceManager;
    BE_NOCOPY(Resource);
private:
    enum
    {
        MaxResourceCount = 4
    };
    mutable minitl::pair<weak<IResourceLoader>, ResourceHandle> m_handles[MaxResourceCount];
private:
    void load(weak<IResourceLoader> loader) const;
    void unload(weak<const IResourceLoader> loader) const;
protected:
    Resource();
    ~Resource();
public:
    const ResourceHandle& getResourceHandle(weak<const IResourceLoader> owner) const;
    ResourceHandle& getResourceHandleForWriting(weak<const IResourceLoader> owner) const;
};

}

#include    <resource/resourceloader.hh>

/*****************************************************************************/
#endif
