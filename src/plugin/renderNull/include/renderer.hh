/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RENDERNULL_RENDERER_HH_
#define BE_RENDERNULL_RENDERER_HH_
/*****************************************************************************/
#include    <graphics/renderer/irenderer.hh>
#include    <system/filesystem.hh>

namespace BugEngine { namespace Graphics { namespace NullRenderer
{

class MeshLoader;
class TextureLoader;
class ShaderLoader;

class Renderer : public IRenderer
{
private:
    weak<const FileSystem>  m_filesystem;
public:
    Renderer(weak<const FileSystem> filesystem);
    ~Renderer();

    u32                             getMaxSimultaneousRenderTargets() const override { return 1; }
    bool                            multithreaded() const override {return true; }

    void                            flush() override;
    uint2                           getScreenSize() override { return uint2(1920,1080); }

    weak<const FileSystem>          filesystem() const  { return m_filesystem; }
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { return ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); return ::operator delete(memory); }
};

}}}

/*****************************************************************************/
#endif
