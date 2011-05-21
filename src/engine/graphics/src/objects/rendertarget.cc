/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/objects/rendertarget.script.hh>

namespace BugEngine { namespace Graphics
{

RenderTarget::RenderTarget(TargetType type, u16 width, u16 height)
    :   m_dimensions(width, height)
    ,   m_type(type)
{
}

RenderTarget::~RenderTarget()
{
}

}}
