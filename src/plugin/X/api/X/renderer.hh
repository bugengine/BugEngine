/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_X_RENDERER_HH_
#define BE_X_RENDERER_HH_
/*****************************************************************************/
#include    <graphics/renderer/renderbackend.hh>
#include    <core/threads/thread.hh>
#include    <core/threads/event.hh>

namespace BugEngine { namespace Graphics { namespace X
{

class Window;

class be_api(X) Renderer : public RenderBackend
{
    friend class Window;
protected:
    ::Display*      m_display;
    int             m_screen;
    ::Window        m_rootWindow;
    ::XVisualInfo*  m_visual;
private:
    static int      xError(::Display* display, XErrorEvent* event);
    ::Window        createWindow(const WindowFlags& flags);
public:
    Renderer();
    ~Renderer();

    uint2           getScreenSize() override;
    const istring&  getWindowClassName() const;

    int             step() const override;

    Display*        display()                       { return m_display; }
};

}}}

/*****************************************************************************/
#endif
