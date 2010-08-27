/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <window.hh>


namespace BugEngine
{
    extern HINSTANCE hDllInstance;
}


namespace BugEngine { namespace Graphics { namespace OpenGL
{

Renderer::~Renderer()
{
    if(m_glContext)
        wglDeleteContext(m_glContext);
}

void Renderer::attachWindow(Window* w)
{
    HDC hDC = GetDC(w->m_window);
    static const PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    GLuint pixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, pixelFormat, &pfd);
    if(!m_glContext)
    {
        m_glContext = wglCreateContext(hDC);
    }
    w->m_glContext = m_glContext;
    //wglShareLists(m_glContext, w->m_glContext);
}

}}}
