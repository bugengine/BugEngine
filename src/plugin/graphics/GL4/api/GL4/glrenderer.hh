/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_GL4_RENDERER_HH_
#define BE_GL4_RENDERER_HH_
/*****************************************************************************/
#include    <plugin/plugin.hh>

namespace BugEngine
{

class RenderTargetDescription;
class RenderWindowDescription;
class MeshDescription;
class TextureDescription;
class ShaderProgramDescription;
class IKernelScheduler;


namespace OpenGL
{

class GLMemoryProvider;
class GLWindow;
struct ShaderExtensions;


class GLRenderer : public Windowing::Renderer
{
    friend class GLWindow;
private:
    class Context;
    mutable scoped<Context>             m_context;
    scoped<GLMemoryProvider>            m_openGLMemoryProvider;
    Plugin::Plugin<IKernelScheduler>    m_openCLScheduler;
public:
    GLRenderer(const Plugin::Context& context);
    ~GLRenderer();

    u32                     getMaxSimultaneousRenderTargets() const override { return 1; }

    void                    flush() override;
public:
    const ShaderExtensions& shaderext() const;
private:
    void                    attachWindow(weak<GLWindow> w) const;
private:
    ref<IGPUResource>       create(weak<const RenderSurfaceDescription> rendersurface) const override;
    ref<IGPUResource>       create(weak<const RenderWindowDescription> renderwindow) const override;
    ref<IGPUResource>       create(weak<const ShaderProgramDescription> shader) const override;
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); ::operator delete(memory); }
};

}}

/*****************************************************************************/
#endif
