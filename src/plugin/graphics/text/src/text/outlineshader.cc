/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.graphics.text/stdafx.h>
#include <bugengine/plugin.graphics.3d/shader/types.script.hh>
#include <bugengine/plugin.graphics.text/outlineshader.script.hh>

namespace BugEngine {

minitl::vector< ref< Shaders::Output > >
createOutlineShader(weak< Shaders::Float4 > textColor, weak< Shaders::Float4 > backgroundColor)
{
    be_forceuse(textColor);
    be_forceuse(backgroundColor);
    minitl::vector< ref< Shaders::Output > > result(Arena::temporary());
    result.reserve(1);
    // result.push_back();
    return result;
}

OutlineShader::OutlineShader(weak< Shaders::Float4 > textColor,
                             weak< Shaders::Float4 > backgroundColor)
    : ShaderProgramDescription(createOutlineShader(textColor, backgroundColor))
{
}

OutlineShader::~OutlineShader()
{
}

}  // namespace BugEngine
