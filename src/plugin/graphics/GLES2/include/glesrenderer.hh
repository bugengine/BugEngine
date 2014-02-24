/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_GLES2_RENDERER_HH_
#define BE_GLES2_RENDERER_HH_
/**************************************************************************************************/
#include    <plugin/plugin.hh>
#include    <3d/renderer/irenderer.hh>

namespace BugEngine { namespace OpenGLES
{

class GLESWindow;


class GLESRenderer : public IRenderer
{
    friend class GLESWindow;
public:
    GLESRenderer(const Plugin::Context& context);
    ~GLESRenderer();

    u32                     getMaxSimultaneousRenderTargets() const override { return 1; }
    void                    flush() override;
private:
    void                    attachWindow(weak<GLESWindow> w) const;
private:
    ref<IGPUResource>       create(weak<const RenderSurfaceDescription> renderSurfaceDescription) const override;
    ref<IGPUResource>       create(weak<const RenderWindowDescription> renderWindowDescription) const override;
    ref<IGPUResource>       create(weak<const ShaderProgramDescription> shaderDescription) const override;
    uint2                   getScreenSize() const override;
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); ::operator delete(memory); }
};

}}

/**************************************************************************************************/
#endif
