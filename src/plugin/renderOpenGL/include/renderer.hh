/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_RENDERER_HH_
#define BE_OPENGL_RENDERER_HH_
/*****************************************************************************/
#include    <system/filesystem.hh>

namespace BugEngine {

class IResourceLoader;

namespace Graphics { namespace OpenGL
{

class GLWindow;
struct ShaderExtensions;


class Renderer : public Windowing::Renderer
{
    friend class GLWindow;
private:
    class Context;
    mutable scoped<Context>         m_context;
    weak<const FileSystem>          m_filesystem;
public:
    Renderer(weak<const FileSystem> filesystem);
    ~Renderer();

    u32                             getMaxSimultaneousRenderTargets() const override { return 1; }
    bool                            multithreaded() const override { return true; }

    void                            flush() override;

    weak<const FileSystem>          filesystem() const  { return m_filesystem; }

public:
    const ShaderExtensions&         shaderext() const;
private:
    void                            attachWindow(weak<GLWindow> w) const;
private:
    ref<IGPUResource>               createRenderTarget(weak<const RenderTarget> rendertarget) override;
    ref<IGPUResource>               createRenderWindow(weak<const RenderWindow> renderwindow) override;
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { return ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); return ::operator delete(memory); }
};

}}}

/*****************************************************************************/
#endif
