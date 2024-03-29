/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_TEXT_OUTLINESHADER_SCRIPT_HH_
#define BE_TEXT_OUTLINESHADER_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.text/stdafx.h>
#include <bugengine/plugin.graphics.3d/shader/shader.script.hh>
#include <bugengine/plugin.graphics.3d/shader/types.script.hh>

namespace BugEngine {

class be_api(TEXT) OutlineShader : public ShaderProgramDescription
{
    friend class OutlineShaderManager;
published:
    OutlineShader(weak< Shaders::Float4 > textColor, weak< Shaders::Float4 > backgroundColor);
    ~OutlineShader();
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
