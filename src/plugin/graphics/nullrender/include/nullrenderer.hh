/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_NULLRENDER_RENDERER_HH_
#define BE_NULLRENDER_RENDERER_HH_
/*****************************************************************************/
#include    <3d/renderer/irenderer.hh>
#include    <system/file/folder.script.hh>
#include    <system/plugin.hh>

namespace BugEngine { namespace Null
{

class NullRenderer : public IRenderer
{
    BE_NOCOPY(NullRenderer);
public:
    NullRenderer(const PluginContext& context);
    ~NullRenderer();

    u32                 getMaxSimultaneousRenderTargets() const override { return 1; }

    void                flush() override;
    uint2               getScreenSize() override { return make_uint2(1920,1080); }
private:
    ref<IGPUResource>   create(weak<const RenderSurface> rendersurface) const override;
    ref<IGPUResource>   create(weak<const RenderWindow> renderwindow) const override;
    ref<IGPUResource>   create(weak<const ShaderProgram> shader) const override;
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); ::operator delete(memory); }
};

}}

/*****************************************************************************/
#endif
