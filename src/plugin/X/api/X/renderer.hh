/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_X_RENDERER_HH_
#define BE_X_RENDERER_HH_
/*****************************************************************************/
#include    <graphics/renderer/irenderer.hh>
#include    <core/threads/thread.hh>
#include    <core/threads/event.hh>

#include    <GL/glx.h>

namespace BugEngine { namespace Graphics { namespace X
{

class Window;

class be_api(X) Renderer : public IRenderer
{
    friend class Window;
protected:
    ::Display*      m_display;
    int             m_screen;
    ::Window        m_rootWindow;
    GLXFBConfig     m_fbConfig;
    ::XVisualInfo*  m_visual;
    ::Atom          m_windowProperty;
protected:
    void flush() override;
private:
    static int      xError(::Display* display, XErrorEvent* event);
    ::Window        createWindow(const WindowFlags& flags);
public:
    Renderer();
    ~Renderer();

    uint2           getScreenSize() override;
    const istring&  getWindowClassName() const;

    Display*        display()                       { return m_display; }
};

}}}

/*****************************************************************************/
#endif
