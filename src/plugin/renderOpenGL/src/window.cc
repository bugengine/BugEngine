/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#include    <stdafx.h>
#include    <window.hh>
#include    <renderer.hh>
#include    <cgshaderparam.hh>

namespace BugEngine
{
    extern HINSTANCE hDllInstance;
}


namespace BugEngine { namespace Graphics { namespace OpenGL
{

Window::Window(Renderer* renderer, WindowFlags flags, const Scene* scene)
:   Windowing::Window(renderer, flags, scene)
,   m_owner(renderer)
,   m_dc(GetDC(m_window))
{
    m_owner->attachWindow(this);
}

Window::~Window()
{
}

bool Window::closed() const
{
    AssertNotReached();
    return false;
}

void Window::setCurrent()
{
#ifdef BE_PLATFORM_WIN32
    wglMakeCurrent(m_dc, m_owner->m_glContext);
#else
    #error
#endif
}

void Window::close()
{
    Win32::Window::close();
}

void Window::begin()
{
    setCurrent();
    uint2 size = getDimensions();
    m_owner->m_systemParams[Renderer::__Screen]->setValue(float4(checked_numcast<float>(size.x()), checked_numcast<float>(size.y())));
}

void Window::end()
{
#ifdef BE_PLATFORM_WIN32
    SwapBuffers(m_dc);
#else
    #error
#endif
}

DebugRenderer* Window::debugRenderer()
{
    return m_owner->debugRenderer();
}

}}}
