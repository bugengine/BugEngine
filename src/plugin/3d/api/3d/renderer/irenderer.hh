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
}

namespace BugEngine { namespace Graphics
{

class SceneGraphLoader;
class RenderTarget;
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
    scoped<SceneGraphLoader>                    m_sceneLoader;
    ref<ITask>                                  m_syncTask;
    scoped< GPUResourceLoader<RenderTarget> >   m_renderTargetLoader;
    scoped< GPUResourceLoader<RenderWindow> >   m_renderWindowLoader;
    scoped< GPUResourceLoader<ShaderProgram> >  m_shaderProgramLoader;
protected:
    IRenderer(Allocator& allocator, weak<ResourceManager> manager, Scheduler::Affinity affinity = Scheduler::DontCare);
    virtual ~IRenderer();
protected:
    virtual void                flush();
    virtual ref<IGPUResource>   createRenderTarget(weak<const RenderTarget> rendertarget) const = 0;
    virtual ref<IGPUResource>   createRenderWindow(weak<const RenderWindow> renderwindow) const = 0;
    //virtual ref<IGPUResource>   createMesh(weak<const Mesh> mesh) const = 0;
    virtual ref<IGPUResource>   createShaderProgram(weak<const ShaderProgram> shader) const = 0;
    //virtual ref<IGPUResource>   createTexture(weak<const Texture> texture) = 0;
public:
            weak<ITask>         syncTask() const;

    virtual uint2               getScreenSize() = 0;

    virtual u32                 getMaxSimultaneousRenderTargets() const = 0;

            Allocator&          arena() const;
};

}}

/*****************************************************************************/
#endif

