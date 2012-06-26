/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_3D_RENDERER_IRENDERER_HH_
#define BE_3D_RENDERER_IRENDERER_HH_
/*****************************************************************************/
#include    <system/resource/resourcemanager.hh>
#include    <3d/renderer/igpuresource.hh>
#include    <system/scheduler/scheduler.hh>

namespace BugEngine
{

class ITask;

class Kernel;
class SceneGraphLoader;
class RenderSurface;
class RenderWindow;
class Mesh;
class ShaderProgram;
class Texture;
class IGPUResource;
template< typename R >
class GPUResourceLoader;

class be_api(_3D) IRenderer : public minitl::pointer
{
    template< typename R >
    friend class GPUResourceLoader;
protected:
    Allocator&                                  m_allocator;
    weak<ResourceManager>                       m_resourceManager;
    ref<ITask>                                  m_syncTask;
    scoped<SceneGraphLoader>                    m_sceneLoader;
    scoped< GPUResourceLoader<RenderSurface> >  m_renderSurfaceLoader;
    scoped< GPUResourceLoader<RenderWindow> >   m_renderWindowLoader;
    scoped< GPUResourceLoader<ShaderProgram> >  m_shaderProgramLoader;
    scoped<Kernel>                              m_kernelSort;
    scoped<Kernel>                              m_kernelRender;
protected:
    IRenderer(Allocator& allocator, weak<ResourceManager> manager, Scheduler::Affinity affinity = Scheduler::DontCare);
    virtual ~IRenderer();
protected:
    virtual void flush();

    virtual ref<IGPUResource> create(weak<const RenderSurface> rendertarget) const = 0;
    virtual ref<IGPUResource> create(weak<const RenderWindow> renderwindow) const = 0;
    //virtual ref<IGPUResource>   create(weak<const Mesh> mesh) const = 0;
    virtual ref<IGPUResource> create(weak<const ShaderProgram> shader) const = 0;
    //virtual ref<IGPUResource>   create(weak<const Texture> texture) = 0;
public:
    weak<IGPUResource> getRenderSurface(weak<const Resource> resource) const;
    weak<IGPUResource> getRenderWindow(weak<const Resource> resource) const;
    weak<IGPUResource> getShaderProgram(weak<const Resource> resource) const;

    Allocator& arena() const;
    weak<ITask> syncTask() const;

    virtual uint2 getScreenSize() = 0;
    virtual u32 getMaxSimultaneousRenderTargets() const = 0;
};

}

/*****************************************************************************/
#endif

