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

#ifndef BE_WIN32_RENDERER_HH_
#define BE_WIN32_RENDERER_HH_
/*****************************************************************************/
#include    <graphics/renderer/renderbackend.hh>
#include    <core/threads/thread.hh>

namespace BugEngine { namespace Graphics { namespace Win32
{

class Window;

struct WindowCreationFlags
{
	const char *className;
	const char *title;
	int x;
	int y;
	RECT size;
	DWORD flags;
	bool fullscreen;
};

class Renderer : public RenderBackend
{
    friend class Window;
private:
    istring         m_windowClassName;
    WNDCLASSEX      m_wndClassEx;
    Thread          m_windowManagementThread;
    volatile bool   m_exit;
private:
    static intptr_t updateWindows(intptr_t p1, intptr_t p2);
private:
    HWND createWindowImplementation(const WindowCreationFlags* flags) const;
    void destroyWindowImplementation(HWND hWnd);
public:
    Renderer();
    ~Renderer();

    uint2           getScreenSize() override;
    const istring&  getWindowClassName() const;

    int             step() const override;
};

}}}

/*****************************************************************************/
#endif
