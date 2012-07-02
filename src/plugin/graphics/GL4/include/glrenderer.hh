/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_RENDERER_HH_
#define BE_OPENGL_RENDERER_HH_
/*****************************************************************************/
#include    <system/plugin.hh>

namespace BugEngine
{

class RenderTarget;
class RenderWindow;
class Mesh;
class Texture;
class Shader;
class IKernelScheduler;


namespace OpenGL
{

class GLWindow;
struct ShaderExtensions;


class GLRenderer : public Windowing::Renderer
{
    friend class GLWindow;
private:
    class Context;
    mutable scoped<Context>     m_context;
    Plugin<IKernelScheduler>    m_openCLScheduler;
public:
    GLRenderer(const PluginContext& context);
    ~GLRenderer();

    u32                     getMaxSimultaneousRenderTargets() const override { return 1; }

    void                    flush() override;
public:
    const ShaderExtensions& shaderext() const;
private:
    void                    attachWindow(weak<GLWindow> w) const;
private:
    ref<IGPUResource>       create(weak<const RenderSurface> rendersurface) const override;
    ref<IGPUResource>       create(weak<const RenderWindow> renderwindow) const override;
    ref<IGPUResource>       create(weak<const ShaderProgram> shader) const override;
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); ::operator delete(memory); }
};

}}

/*****************************************************************************/
#endif
