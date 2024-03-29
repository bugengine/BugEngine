/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <stdafx.h>
#include <loaders/dx9shader.hh>
//#include    <loaders/dx9shaderbuilder.hh>
#include <bugengine/plugin.graphics.3d/shader/shader.script.hh>

namespace BugEngine { namespace DirectX9 {

Dx9ShaderProgram::Dx9ShaderProgram(weak< const ShaderProgramDescription > shaderDescription,
                                   weak< const Dx9Renderer >              renderer)
    : IGPUResource(shaderDescription, renderer)
{
}

Dx9ShaderProgram::~Dx9ShaderProgram()
{
}

void Dx9ShaderProgram::load(weak< const Resource::Description > shaderDescription)
{
    be_forceuse(shaderDescription);
}

void Dx9ShaderProgram::unload()
{
}

}}  // namespace BugEngine::DirectX9
