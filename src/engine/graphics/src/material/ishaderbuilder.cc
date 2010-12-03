/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/material/ishaderbuilder.hh>

namespace BugEngine { namespace Graphics
{

IShaderBuilder::IShaderBuilder()
    :   m_stream(tempArena(), 10000)
{
}

IShaderBuilder::~IShaderBuilder()
{
}

}}
