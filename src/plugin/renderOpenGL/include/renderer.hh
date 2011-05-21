/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_RENDERER_HH_
#define BE_OPENGL_RENDERER_HH_
/*****************************************************************************/
#include    <system/filesystem.hh>

namespace BugEngine {

class Resource;

namespace Graphics { namespace OpenGL
{

class MeshLoader;
class TextureLoader;
class ShaderLoader;
class Window;
struct ShaderExtensions;


class Renderer : public Windowing::Renderer
{
    friend class Window;
public:
    class GPUResource : public minitl::inode, public minitl::intrusive_list<GPUResource>::item
    {
    public:
        GPUResource(weak<const Resource> from);
        virtual ~GPUResource();

        const weak<const Resource>  owner;
        GLuint                      resource;

        void load();
        void unload();
    private:
        virtual GLuint doLoad() = 0;
        virtual void doUnload() = 0;
    };
private:
    class Context;
    scoped<Context>             m_context;
    weak<const FileSystem>      m_filesystem;
    scoped<const MeshLoader>    m_meshLoader;
    scoped<const TextureLoader> m_textureLoader;
    scoped<const ShaderLoader>  m_shaderLoader;
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
