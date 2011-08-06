/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_IRENDERER_HH_
#define BE_GRAPHICS_RENDERER_IRENDERER_HH_
/*****************************************************************************/
#include    <system/resource/resourcehandle.hh>
#include    <graphics/renderer/igpuresource.hh>
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
class VertexShader;
class GeometryShader;
class FragmentShader;
class Texture;
class IGPUResource;

class be_api(GRAPHICS) IRenderer : public minitl::pointer
{
protected:
    Allocator&                                  m_allocator;
    scoped<SceneGraphLoader>                    m_sceneLoader;
    ref<ITask>                                  m_syncTask;
    minitl::vector< ref<minitl::refcountable> > m_deletedObjects;
    minitl::intrusive_list<IGPUResource>        m_pendingRenderTargets;
    minitl::intrusive_list<IGPUResource>        m_pendingShaders;
protected:
    IRenderer(Allocator& allocator, Scheduler::Affinity affinity = Scheduler::DontCare);
    virtual ~IRenderer();
private:
            void            destroy(const ResourceHandle& r);
            ResourceHandle  load(weak<const RenderTarget> rendertarget);
            ResourceHandle  load(weak<const RenderWindow> renderwindow);
    //        ResourceHandle  load(weak<const Mesh> mesh);
            ResourceHandle  load(weak<const ShaderProgram> program);
    //        ResourceHandle  load(weak<const Texture> texture);
protected:
    virtual void                flush();
    virtual ref<IGPUResource>   createRenderTarget(weak<const RenderTarget> rendertarget) = 0;
    virtual ref<IGPUResource>   createRenderWindow(weak<const RenderWindow> renderwindow) = 0;
    //virtual ref<IGPUResource>   createMesh(weak<const Mesh> mesh) = 0;
    virtual ref<IGPUResource>   createShaderProgram(weak<const ShaderProgram> shader) = 0;
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

