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

class be_api(X) Window : public RenderTarget
{
private:
    class Attributes
    {
        friend class Window;
    private:
        ::XSetWindowAttributes  m_attributes;
        int                     m_attributeMask;
    public:
        Attributes(::Display* display, ::Window root, ::Visual* visual, WindowFlags flags);
        ~Attributes();
    };
protected:
    weak<Renderer>      m_renderer;
    Attributes          m_attributes;
    ::Window            m_window;
public:
    Window(weak<Renderer> renderer,WindowFlags flags, ref<const Scene> scene);
    ~Window();

    void close() override;

    uint2 getDimensions() const override;
};

}}}

/*****************************************************************************/
#endif
