/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_IRENDERER_HH_
#define BE_GRAPHICS_RENDERER_IRENDERER_HH_
/*****************************************************************************/
#include    <system/resource/resourcehandle.hh>
#include    <graphics/renderer/igpuresource.hh>

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
class Shader;
class Texture;
class IGPUResource;

class be_api(GRAPHICS) IRenderer : public minitl::pointer
{
private:
    Allocator&                              m_allocator;
    scoped<SceneGraphLoader>                m_sceneLoader;
    ref<ITask>                              m_syncTask;
    minitl::intrusive_list<IGPUResource>    m_resources;
protected:
    IRenderer(Allocator& allocator);
    virtual ~IRenderer();
private:
            void            destroy(const ResourceHandle& r);
            ResourceHandle  load(weak<const RenderTarget> rendertarget);
            ResourceHandle  load(weak<const RenderWindow> renderwindow);
    //        ResourceHandle  load(weak<const Mesh> mesh);
    //        ResourceHandle  load(weak<const Shader> shader);
    //        ResourceHandle  load(weak<const Texture> texture);
protected:
    virtual void                flush() = 0;
    virtual ref<IGPUResource>   createRenderTarget(weak<const RenderTarget> rendertarget) = 0;
    virtual ref<IGPUResource>   createRenderWindow(weak<const RenderWindow> renderwindow) = 0;
    //virtual ref<IGPUResource>   createMesh(weak<const Mesh> mesh) = 0;
    //virtual ref<IGPUResource>   createShader(weak<const Shader> shader) = 0;
    //virtual ref<IGPUResource>   createTexture(weak<const Texture> texture) = 0;
public:
            weak<ITask>         syncTask() const;

    virtual uint2               getScreenSize() = 0;

    virtual u32                 getMaxSimultaneousRenderTargets() const = 0;
    virtual bool                multithreaded() const = 0;

            Allocator&          arena() const;
};

}}

/*****************************************************************************/
#endif

