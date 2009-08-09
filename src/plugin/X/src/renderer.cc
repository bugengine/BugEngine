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

#include    <X/stdafx.h>
#include    <X/renderer.hh>
#include    <X/window.hh>
#include    <core/threads/event.hh>

namespace BugEngine { namespace Graphics { namespace X
{

namespace
{
}

Renderer::Renderer()
:   m_windowManagementThread("WindowManagement", &Renderer::updateWindows, (intptr_t)&m_exit, 0, Thread::AboveNormal)
,   m_exit(false)
{
}

Renderer::~Renderer()
{
}

uint2 Renderer::getScreenSize()
{
    return uint2(1920, 1200);
}

int Renderer::step() const
{
    return (int)m_exit;
}

intptr_t Renderer::updateWindows(intptr_t p1, intptr_t p2)
{
    return 0;
}

}}}
