/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/objects/shader.script.hh>

namespace BugEngine { namespace Graphics
{

Shader::Shader(ShaderType type, ref<Shaders::Method> main)
    :   main(main)
    ,   type(type)
{
}

Shader::~Shader()
{
}

void Shader::buildSource(IShaderBuilder& builder) const
{
    if (main) main->buildSource(builder);
}

}}
