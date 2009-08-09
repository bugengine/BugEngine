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

#ifndef BE_OPENGL_WINDOW_HH_
#define BE_OPENGL_WINDOW_HH_
/*****************************************************************************/
#include    <graphics/renderer/renderbackend.hh>


namespace BugEngine { namespace Graphics { namespace OpenGL
{

class Renderer;

class Window : public Windowing::Window
{
    friend Renderer;
private:
    Renderer*               m_owner;
    HDC                     m_dc;
public:
    Window(Renderer* renderer, WindowFlags flags, const Scene* scene);
    ~Window();

    void setCurrent() override;

    void begin();
    void end();

    void close() override;

    bool closed() const override;

    DebugRenderer* debugRenderer() override;
};

}}}

/*****************************************************************************/
#endif
