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

class Window;
struct ShaderExtensions;


class Renderer : public Windowing::Renderer
{
    friend class Window;
private:
    class Context;
    scoped<Context>                 m_context;
    weak<const FileSystem>          m_filesystem;
    scoped<const IResourceLoader>   m_renderTargetLoader;
    scoped<const IResourceLoader>   m_meshLoader;
    scoped<const IResourceLoader>   m_textureLoader;
    scoped<const IResourceLoader>   m_shaderLoader;
public:
    Renderer(weak<const FileSystem> filesystem);
    ~Renderer();

    ref<Graphics::IRenderTarget>    createRenderWindow(WindowFlags flags) override;
    ref<Graphics::IRenderTarget>    createRenderBuffer(TextureFlags flags) override;
    ref<Graphics::IRenderTarget>    createMultipleRenderBuffer(TextureFlags flags, size_t count) override;

    u32                             getMaxSimultaneousRenderTargets() const override { return 1; }
    bool                            multithreaded() const override {return true; }

    void                            flush() override;

    weak<const FileSystem>          filesystem() const  { return m_filesystem; }

public:
    const ShaderExtensions&         shaderext() const;
private:
    void                            attachWindow(Window* w);
protected:
    void                            createContext(void* params) override;
    void                            destroyContext() override;
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { return ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); return ::operator delete(memory); }
};

}}}

/*****************************************************************************/
#endif
