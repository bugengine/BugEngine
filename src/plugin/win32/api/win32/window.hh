/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WIN32_WINDOW_HH_
#define BE_WIN32_WINDOW_HH_
/*****************************************************************************/
#include    <graphics/renderer/rendertarget.hh>

namespace BugEngine { namespace Graphics { namespace Win32
{

class Renderer;

class Window : public RenderTarget
{
protected:
    Renderer*   m_renderer;
    HWND        m_window;
    HDC         m_dc;
public:
    Window(Renderer* renderer,WindowFlags flags, const Scene* scene);
    ~Window();

    void close() override;

    uint2 getDimensions() const override;
};

}}}

/*****************************************************************************/
#endif
