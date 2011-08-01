/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/objects/shaders/param.script.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{

Uniform::Uniform(const istring& name)
    :   name(name)
{
}

Uniform::~Uniform()
{
}

void Uniform::buildDeclarations(IShaderBuilder& /*stream*/) const
{
}

void Uniform::buildDefinitions(IShaderBuilder& /*stream*/) const
{
}

}}}
