/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RESOURCE_DESCRIPTION_SCRIPT_HH_
#define BE_RESOURCE_DESCRIPTION_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/resource/stdafx.h>
#include <bugengine/minitl/intrusive_list.hh>
#include <bugengine/minitl/refcountable.hh>
#include <bugengine/resource/resource.hh>

namespace BugEngine { namespace Resource {

class ILoader;
class ResourceManager;

class be_api(RESOURCE) Description
    : public minitl::refcountable
    , public minitl::intrusive_list< const Description, 2 >::item
{
    friend class ResourceManager;
    BE_NOCOPY(Description);

private:
    enum
    {
        MaxResourceCount = 2
    };
    union ResourceCache
    {
        Resource* m_resourcePointer;
        u8        m_resourceBuffer[MaxResourceCount * sizeof(Resource)];
    };
    mutable u32           m_resourceCount;
    mutable ResourceCache m_resourceCache;

private:
    Resource* getResourceBuffer() const;
    Resource& getResourceForWriting(weak< const ILoader > owner) const;
    void      load(weak< ILoader > loader) const;
    void      unload(weak< ILoader > loader) const;

protected:
    Description();
    ~Description();

public:
    const Resource& getResource(weak< const ILoader > owner) const;
};

}}  // namespace BugEngine::Resource

/**************************************************************************************************/
#endif
