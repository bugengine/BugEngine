/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/objects/shader.script.hh>
#include    <graphics/objects/shaders/method.script.hh>

namespace BugEngine { namespace Graphics
{

ShaderProgram::ShaderProgram(ref<const Shaders::Node> position, ref<const Shaders::Node> color, ref<const Shaders::Node> depth)
    :   position(position)
    ,   color(color)
    ,   depth(depth)
{
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::buildSource(Shaders::IShaderBuilder& builder, Shaders::Stage stage) const
{
    if (position) position->buildDeclarations(builder, Shaders::VertexStage, stage);
    if (color) color->buildDeclarations(builder, Shaders::FragmentStage, stage);
    if (depth) depth->buildDeclarations(builder, Shaders::FragmentStage, stage);

    if (position) position->buildDefinitions(builder, Shaders::VertexStage, stage);
    if (color) color->buildDefinitions(builder, Shaders::FragmentStage, stage);
    if (depth) depth->buildDefinitions(builder, Shaders::FragmentStage, stage);
}

}}
