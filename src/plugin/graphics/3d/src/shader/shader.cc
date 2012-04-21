/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <3d/stdafx.h>
#include    <3d/shader/shader.script.hh>
#include    <3d/shader/ishaderbuilder.hh>

namespace BugEngine
{

ShaderProgram::ShaderProgram()
{
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::buildSource(Shaders::IShaderBuilder& /*builder*/, Shaders::Stage /*stage*/) const
{
/*
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
*/
}

}
