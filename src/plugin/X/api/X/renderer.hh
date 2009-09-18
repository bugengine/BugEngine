/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_X_RENDERER_HH_
#define BE_X_RENDERER_HH_
/*****************************************************************************/
#include    <graphics/renderer/renderbackend.hh>
#include    <core/threads/thread.hh>

namespace BugEngine { namespace Graphics { namespace X
{

class Window;

class XEXPORT Renderer : public RenderBackend
{
    friend class Window;
private:
    Thread          m_windowManagementThread;
    volatile bool   m_exit;
protected:
    Display*        m_display;
private:
    static intptr_t updateWindows(intptr_t p1, intptr_t p2);
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
