/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <loaders/shader/glshaderbuilder.hh>


namespace BugEngine { namespace Graphics { namespace OpenGL
{

GLShaderBuilder::GLShaderBuilder()
{
    write("#version 140");
}

GLShaderBuilder::~GLShaderBuilder()
{
}

void GLShaderBuilder::doAddVariableDeclaration(const istring& /*name*/, Shaders::Scope /*scope*/, Shaders::Type /*type*/, Shaders::Semantic /*semantic*/)
{
}

}}}
