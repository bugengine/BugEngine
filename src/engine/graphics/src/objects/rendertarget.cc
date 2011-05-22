/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/objects/rendertarget.script.hh>

namespace BugEngine { namespace Graphics
{

RenderTarget::RenderTarget(u16 width, u16 height)
    :   m_dimensions(width, height)
    ,   m_type(Buffer)
    ,   m_title()
    ,   m_fullscreen(false)
{
}

RenderTarget::RenderTarget(u16 width, u16 height, istring title, bool fullscreen)
    :   m_dimensions(width, height)
    ,   m_type(Buffer)
    ,   m_title(title)
    ,   m_fullscreen(false)
{
}

RenderTarget::~RenderTarget()
{
}

}}
