/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WIN32_RENDERER_HH_
#define BE_WIN32_RENDERER_HH_
/*****************************************************************************/
#include    <graphics/renderer/irenderer.hh>
#include    <core/threads/thread.hh>

namespace BugEngine { namespace Graphics { namespace Win32
{

class Window;

class Renderer : public IRenderer
{
    friend class Window;
private:
    istring         m_windowClassName;
    WNDCLASSEX      m_wndClassEx;
protected:
    void            flush() override;
public:
    Renderer();
    ~Renderer();

    uint2           getScreenSize() override;
    const istring&  getWindowClassName() const;
};

}}}

/*****************************************************************************/
#endif
