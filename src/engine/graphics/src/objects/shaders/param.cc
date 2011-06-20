/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/objects/shaders/param.script.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{

Param::Param(const istring& name, ref<const Node> value)
    :   name(name)
    ,   value(value)
{
}

Param::~Param()
{
}


void Param::buildSource(IShaderBuilder& /*stream*/) const
{
}

Uniform::Uniform(const istring& name)
    :   name(name)
{
}

Uniform::~Uniform()
{
}

void Uniform::buildSource(IShaderBuilder& /*stream*/) const
{
}

}}}
