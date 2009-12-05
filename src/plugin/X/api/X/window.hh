/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_X_WINDOW_HH_
#define BE_X_WINDOW_HH_
/*****************************************************************************/
#include    <graphics/renderer/rendertarget.hh>
#include    <graphics/renderer/renderbackend.hh>

namespace BugEngine { namespace Graphics { namespace X
{

class Renderer;

class XEXPORT Window : public RenderTarget
{
protected:
    weak<Renderer>  m_renderer;
    ::Window        m_window;
public:
    Window(weak<Renderer> renderer,WindowFlags flags, ref<const Scene> scene);
    ~Window();

    void close() override;

    uint2 getDimensions() const override;
};

}}}

/*****************************************************************************/
#endif
