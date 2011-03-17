/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/material/shader.script.hh>

namespace BugEngine { namespace Graphics
{

Shader::Shader()
{
}

Shader::~Shader()
{
}

void Shader::buildSource(const IShaderBuilder& builder) const
{
    m_root->buildSource(builder);
}

}}
