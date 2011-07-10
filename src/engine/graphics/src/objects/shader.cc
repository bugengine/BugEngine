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
                            ref<const Shaders::Node> diffuse,
                            ref<const Shaders::Node> specular,
                            minitl::array< ref<const Shaders::Node>, 18 > varying)
    :   position(position)
    ,   diffuse(diffuse)
    ,   specular(specular)
    ,   varying(varying)
{
}

VertexShader::~VertexShader()
{
}

void VertexShader::buildSource(IShaderBuilder& /*builder*/) const
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


FragmentShader::FragmentShader( ref<const Shaders::Node> color, ref<const Shaders::Node> depth)
    :   color(color)
    ,   depth(depth)
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
