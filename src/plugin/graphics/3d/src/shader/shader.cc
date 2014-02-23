/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <3d/stdafx.h>
#include    <3d/shader/shader.script.hh>
#include    <3d/shader/types.script.hh>
#include    <3d/shader/ishaderbuilder.hh>

namespace BugEngine
{

ShaderProgramDescription::ShaderProgramDescription(minitl::vector< ref<Shaders::Output> > outputs)
    :   m_outputs(outputs)
{
}

ShaderProgramDescription::~ShaderProgramDescription()
{
}

void ShaderProgramDescription::buildSource(Shaders::IShaderBuilder& /*builder*/) const
{
}

}
