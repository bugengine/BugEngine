/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_3D_RENDERER_GPURESOURCELOADER_HH_
#define BE_3D_RENDERER_GPURESOURCELOADER_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.3d/stdafx.h>
#include <bugengine/resource/loader.hh>

namespace BugEngine {

class IRenderer;
class IGPUResource;

template < typename R >
class GPUResourceLoader : public Resource::ILoader
{
    friend class IRenderer;
    BE_NOCOPY(GPUResourceLoader);

private:
    weak< const IRenderer >                m_renderer;
    minitl::intrusive_list< IGPUResource > m_pending;
    minitl::intrusive_list< IGPUResource > m_resources;
    minitl::vector< weak< IGPUResource > > m_deleted;

public:
    GPUResourceLoader(weak< const IRenderer > renderer);
    ~GPUResourceLoader();

protected:
    virtual void load(weak< const Resource::Description > description,
                      Resource::Resource&                 resource) override;
    virtual void reload(weak< const Resource::Description > oldDescription,
                        weak< const Resource::Description > newDescription,
                        Resource::Resource&                 resource) override;
    virtual void unload(Resource::Resource& handle) override;
    void         flush();
};

}  // namespace BugEngine

#include <gpuresourceloader.inl>

/**************************************************************************************************/
#endif
