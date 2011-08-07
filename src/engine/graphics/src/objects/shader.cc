/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/objects/shader.script.hh>
#include    <graphics/objects/shaders/method.script.hh>
#include    <graphics/tools/ishaderbuilder.hh>

namespace BugEngine { namespace Graphics
{

ShaderProgram::ShaderProgram(ref<const Shaders::Float4> position, ref<const Shaders::Float4> color, ref<const Shaders::Float> depth)
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
    if (color) color->buildDeclarations(builder, Shaders::FragmentStage, stage);
    if (depth) depth->buildDeclarations(builder, Shaders::FragmentStage, stage);
    if (position) position->buildDeclarations(builder, Shaders::VertexStage, stage);

    builder.beginMethodDefinition("main");
    if (color)
    {
        color->buildDefinitions(builder, Shaders::FragmentStage, stage);
        if (stage == Shaders::FragmentStage)
            builder.saveTo(Shaders::Color, color);
    }
    if (depth)
    {
        depth->buildDefinitions(builder, Shaders::FragmentStage, stage);
        if (stage == Shaders::FragmentStage)
            builder.saveTo(Shaders::Depth, depth);
    }
    if (position)
    {
        position->buildDefinitions(builder, Shaders::VertexStage, stage);
        if (stage == Shaders::VertexStage)
            builder.saveTo(Shaders::Position, position);
    }
    builder.forwardAttributes();
    builder.end();
}

}}
