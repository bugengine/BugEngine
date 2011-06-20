/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RENDERNULL_RENDERER_HH_
#define BE_RENDERNULL_RENDERER_HH_
/*****************************************************************************/
#include    <graphics/renderer/irenderer.hh>
#include    <system/filesystem.hh>

namespace BugEngine { namespace Graphics { namespace Null
{

class NullRenderer : public IRenderer
{
private:
    weak<const FileSystem>  m_filesystem;
public:
    NullRenderer(weak<const FileSystem> filesystem);
    ~NullRenderer();

    u32                             getMaxSimultaneousRenderTargets() const override { return 1; }

    void                            flush() override;
    uint2                           getScreenSize() override { return uint2(1920,1080); }

    weak<const FileSystem>          filesystem() const  { return m_filesystem; }
private:
    ref<IGPUResource>               createRenderTarget(weak<const RenderTarget> rendertarget) override;
    ref<IGPUResource>               createRenderWindow(weak<const RenderWindow> renderwindow) override;
    ref<IGPUResource>               createShaderProgram(weak<const ShaderProgram> shader) override;
    ref<IGPUResource>               createVertexShader(weak<const VertexShader> shader) override;
    ref<IGPUResource>               createGeometryShader(weak<const GeometryShader> shader) override;
    ref<IGPUResource>               createFragmentShader(weak<const FragmentShader> shader) override;
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { return ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); return ::operator delete(memory); }
};

}}}

/*****************************************************************************/
#endif
