/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_IGPULOADER_HH_
#define BE_GRAPHICS_RENDERER_IGPULOADER_HH_
/*****************************************************************************/
#include    <system/resource/iresourceloader.script.hh>
#include    <graphics/renderer/irenderer.hh>

namespace BugEngine { namespace Graphics
{

template< typename SystemResource, typename GPUResource >
class GPUResourceLoader : public IResourceLoader
{
protected:
    const weak<IRenderer> m_renderer;
public:
    GPUResourceLoader(weak<IRenderer> renderer) :   m_renderer(renderer) { attach<SystemResource>(); }
    ~GPUResourceLoader()                                                 { detach<SystemResource>(); }

    virtual void* load(weak<const Resource> source) override
    {
        GPUResource* resource = new GPUResource(source);
        return resource;
    }
    virtual void  unload(const void* resource) override
    {
        const GPUResource* gpuresource = (const GPUResource*) resource;
        delete gpuresource;
    }
};

}}

/*****************************************************************************/
#endif
