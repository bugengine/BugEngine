/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.graphics.3d/stdafx.h>
#include <bugengine/plugin.graphics.3d/shader/ishaderbuilder.hh>
#include <bugengine/plugin.graphics.3d/shader/shader.script.hh>
#include <bugengine/plugin.graphics.3d/shader/types.script.hh>

namespace BugEngine {

ShaderProgramDescription::ShaderProgramDescription(minitl::vector< ref< Shaders::Output > > outputs)
    : m_outputs(outputs)
{
}

ShaderProgramDescription::~ShaderProgramDescription()
{
}

void ShaderProgramDescription::buildSource(Shaders::IShaderBuilder& /*builder*/) const
{
}

}  // namespace BugEngine
