/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RENDERNULL_RENDERER_HH_
#define BE_RENDERNULL_RENDERER_HH_
/*****************************************************************************/
#include    <3d/renderer/irenderer.hh>
#include    <system/file/folder.script.hh>

namespace BugEngine { namespace Graphics { namespace Null
{

class NullRenderer : public IRenderer
{
    BE_NOCOPY(NullRenderer);
private:
    weak<const Folder>  m_dataFolder;
public:
    NullRenderer(weak<const Folder> dataFolder);
    ~NullRenderer();

    u32                 getMaxSimultaneousRenderTargets() const override { return 1; }

    void                flush() override;
    uint2               getScreenSize() override { return uint2(1920,1080); }

    weak<const Folder>  dataFolder() const  { return m_dataFolder; }
private:
    ref<IGPUResource>   createRenderTarget(weak<const RenderTarget> rendertarget) override;
    ref<IGPUResource>   createRenderWindow(weak<const RenderWindow> renderwindow) override;
    ref<IGPUResource>   createShaderProgram(weak<const ShaderProgram> shader) override;
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { return ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); return ::operator delete(memory); }
};

}}}

/*****************************************************************************/
#endif
