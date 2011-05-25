/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_IRENDERER_HH_
#define BE_GRAPHICS_RENDERER_IRENDERER_HH_
/*****************************************************************************/

namespace BugEngine
{
class ITask;
class IResourceLoader;
}

namespace BugEngine { namespace Graphics
{

class be_api(GRAPHICS) IRenderer : public minitl::pointer
{
private:
    Allocator&              m_allocator;
    scoped<IResourceLoader> m_sceneLoader;
    ref<ITask>              m_syncTask;
protected:
    IRenderer(Allocator& allocator);
    virtual ~IRenderer();
protected:
    virtual void                    flush() = 0;
public:
            weak<ITask>             syncTask() const;

    virtual uint2                   getScreenSize() = 0;

    virtual u32                     getMaxSimultaneousRenderTargets() const = 0;
    virtual bool                    multithreaded() const = 0;

            Allocator&              arena() const;
};

}}

/*****************************************************************************/
#endif

