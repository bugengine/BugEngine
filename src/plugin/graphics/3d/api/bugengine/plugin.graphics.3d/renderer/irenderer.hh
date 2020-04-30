/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_3D_RENDERER_IRENDERER_HH_
#define BE_3D_RENDERER_IRENDERER_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.3d/stdafx.h>
#include <bugengine/plugin.graphics.3d/renderer/igpuresource.hh>
#include <bugengine/resource/resourcemanager.hh>
#include <bugengine/scheduler/scheduler.hh>

namespace BugEngine {

namespace Task {
class ITask;
}

namespace KernelScheduler {
class Kernel;
}

class RenderSurfaceDescription;
class RenderWindowDescription;
class MeshDescription;
class ShaderProgramDescription;
class TextureDescription;
class IGPUResource;
template < typename R >
class GPUResourceLoader;

class be_api(3D) IRenderer : public minitl::refcountable
{
    template < typename R >
    friend class GPUResourceLoader;

protected:
    minitl::Allocator&                                      m_allocator;
    weak< Resource::ResourceManager >                       m_resourceManager;
    ref< Task::ITask >                                      m_syncTask;
    scoped< GPUResourceLoader< RenderSurfaceDescription > > m_renderSurfaceLoader;
    scoped< GPUResourceLoader< RenderWindowDescription > >  m_renderWindowLoader;
    scoped< GPUResourceLoader< ShaderProgramDescription > > m_shaderProgramLoader;
    scoped< KernelScheduler::Kernel >                       m_kernelSort;
    scoped< KernelScheduler::Kernel >                       m_kernelRender;

protected:
    IRenderer(minitl::Allocator & allocator, weak< Resource::ResourceManager > manager,
              Scheduler::Affinity affinity = Scheduler::WorkerThread);
    virtual ~IRenderer();

protected:
    virtual void flush();

    virtual ref< IGPUResource > create(weak< const RenderSurfaceDescription > rendertarget) const
       = 0;
    virtual ref< IGPUResource > create(weak< const RenderWindowDescription > renderwindow) const
       = 0;
    // virtual ref<IGPUResource>   create(weak<const Mesh> mesh) const = 0;
    virtual ref< IGPUResource > create(weak< const ShaderProgramDescription > shader) const = 0;
    // virtual ref<IGPUResource>   create(weak<const Texture> texture) = 0;
public:
    weak< IGPUResource > getRenderSurface(weak< const Resource::Description > description) const;
    weak< IGPUResource > getRenderWindow(weak< const Resource::Description > description) const;
    weak< IGPUResource > getShaderProgram(weak< const Resource::Description > description) const;

    minitl::Allocator&  arena() const;
    weak< Task::ITask > syncTask() const;

    virtual uint2 getScreenSize() const                   = 0;
    virtual u32   getMaxSimultaneousRenderTargets() const = 0;
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
