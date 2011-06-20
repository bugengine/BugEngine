/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/objects/shader.script.hh>
#include    <graphics/objects/shaders/method.script.hh>

namespace BugEngine { namespace Graphics
{

Shader::Shader()
{
}

Shader::~Shader()
{
}


VertexShader::VertexShader( ref<const Shaders::Node> position,
                            ref<const Shaders::Node> color,
                            ref<const Shaders::Node> uv1,
                            ref<const Shaders::Node> uv2)
    :   position(position)
    ,   color(color)
    ,   uv1(uv1)
    ,   uv2(uv2)
{
}

VertexShader::~VertexShader()
{
}

void VertexShader::buildSource(IShaderBuilder& /*stream*/) const
{
}


GeometryShader::GeometryShader()
{
}

GeometryShader::~GeometryShader()
{
}

void GeometryShader::buildSource(IShaderBuilder& /*stream*/) const
{
}


FragmentShader::FragmentShader( ref<const Shaders::Node> position,
                            ref<const Shaders::Node> color,
                            ref<const Shaders::Node> uv1,
                            ref<const Shaders::Node> uv2)
    :   position(position)
    ,   color(color)
    ,   uv1(uv1)
    ,   uv2(uv2)
{
}

FragmentShader::~FragmentShader()
{
}

void FragmentShader::buildSource(IShaderBuilder& /*stream*/) const
{
}


ShaderProgram::ShaderProgram(ref<const VertexShader> vertex, ref<const GeometryShader> geometry, ref<const FragmentShader> fragment)
    :   vertex(vertex)
    ,   geometry(geometry)
    ,   fragment(fragment)
{
}

ShaderProgram::~ShaderProgram()
{
}

}}
