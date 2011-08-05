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
                            minitl::array< ref<const Shaders::Node>, 18 > input,
                            minitl::array< ref<const Shaders::Node>, 18 > output)
    :   position(position)
    ,   diffuse(diffuse)
    ,   specular(specular)
    ,   input(input)
    ,   output(output)
{
}

VertexShader::~VertexShader()
{
}

void VertexShader::buildSource(Shaders::IShaderBuilder& builder) const
{
    if (position) position->buildDeclarations(builder);
    if (diffuse) diffuse->buildDeclarations(builder);
    if (specular) specular->buildDeclarations(builder);
    for (minitl::array< ref<const Shaders::Node>, 18 >::const_iterator it = varying.begin(); it != input.end(); ++it)
        if (*it) (*it)->buildDeclarations(builder);
    for (minitl::array< ref<const Shaders::Node>, 18 >::const_iterator it = varying.begin(); it != output.end(); ++it)
        if (*it) (*it)->buildDeclarations(builder);



    if (position) position->buildDefinitions(builder);
    if (diffuse) diffuse->buildDefinitions(builder);
    if (specular) specular->buildDefinitions(builder);
    for (minitl::array< ref<const Shaders::Node>, 18 >::const_iterator it = varying.begin(); it != input.end(); ++it)
        if (*it) (*it)->buildDefinitions(builder);
    for (minitl::array< ref<const Shaders::Node>, 18 >::const_iterator it = varying.begin(); it != output.end(); ++it)
        if (*it) (*it)->buildDefinitions(builder);
}


GeometryShader::GeometryShader()
{
}

GeometryShader::~GeometryShader()
{
}

void GeometryShader::buildSource(Shaders::IShaderBuilder& /*stream*/) const
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

void FragmentShader::buildSource(Shaders::IShaderBuilder& /*stream*/) const
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
