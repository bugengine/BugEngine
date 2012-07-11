/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <loaders/dx9shader.hh>
//#include    <loaders/dx9shaderbuilder.hh>
#include    <3d/shader/shader.script.hh>

namespace BugEngine { namespace DirectX9
{

Dx9ShaderProgram::Dx9ShaderProgram(weak<const ShaderProgram> resource, weak<const Dx9Renderer> renderer)
    :   IGPUResource(resource, renderer)
{
}

Dx9ShaderProgram::~Dx9ShaderProgram()
{
}

void Dx9ShaderProgram::load(weak<const Resource> resource)
{
}

void Dx9ShaderProgram::unload()
{
}

}}
