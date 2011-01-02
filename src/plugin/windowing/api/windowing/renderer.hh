/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WINDOWING_RENDERER_HH_
#define BE_WINDOWING_RENDERER_HH_
/*****************************************************************************/
#include    <graphics/renderer/irenderer.hh>

namespace BugEngine { namespace Graphics { namespace Windowing
{

class Window;
struct WindowCreationFlags;

class Renderer : public IRenderer
{
    friend class Window;
private:
    class PlatformRenderer;
    friend class PlatformRenderer;
    scoped<PlatformRenderer>    m_platformRenderer;
protected:
    void            flush() override;
    void            createContextAsync(void* params);
    void            destroyContextAsync();
public:
    Renderer();
    ~Renderer();

    uint2           getScreenSize() override;

    virtual void    createContext(void* params) = 0;
    virtual void    destroyContext() = 0;
};

}}}

/*****************************************************************************/
#endif
