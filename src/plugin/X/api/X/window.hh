/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_X_WINDOW_HH_
#define BE_X_WINDOW_HH_
/*****************************************************************************/
#include    <graphics/renderer/irendertarget.hh>
#include    <graphics/renderer/irenderer.hh>

namespace BugEngine { namespace Graphics { namespace X
{

class Renderer;

class be_api(X) Window : public IRenderTarget
{
    friend class Renderer;
protected:
    ::Window            m_window;
public:
    Window(weak<Renderer> renderer,WindowFlags flags);
    ~Window();

    void close() override;

    uint2 getDimensions() const override;
};

}}}

/*****************************************************************************/
#endif
