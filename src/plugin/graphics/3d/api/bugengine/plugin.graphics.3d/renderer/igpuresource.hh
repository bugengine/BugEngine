/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_3D_RENDERER_IGPURESOURCE_HH_
#define BE_3D_RENDERER_IGPURESOURCE_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.3d/stdafx.h>
#include <bugengine/minitl/intrusive_list.hh>
#include <bugengine/minitl/refcountable.hh>

namespace BugEngine {

namespace Resource {
class Description;
}

template < typename R >
class GPUResourceLoader;
class IRenderer;

class be_api(3D) IGPUResource
    : public minitl::refcountable
    , public minitl::intrusive_list< IGPUResource >::item
{
    template < typename T >
    friend class GPUResourceLoader;
    BE_NOCOPY(IGPUResource);

protected:
    const weak< const IRenderer >       m_renderer;
    weak< const Resource::Description > m_resource;

private:
    i32 m_index;

public:
    IGPUResource(weak< const Resource::Description > description, weak< const IRenderer > renderer);
    virtual ~IGPUResource();

    virtual void load(weak< const Resource::Description > description) = 0;
    virtual void unload()                                              = 0;

    weak< const Resource::Description > resource() const
    {
        return m_resource;
    }
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
