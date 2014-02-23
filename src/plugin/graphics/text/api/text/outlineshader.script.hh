/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_TEXT_OUTLINESHADER_SCRIPT_HH_
#define BE_TEXT_OUTLINESHADER_SCRIPT_HH_
/*****************************************************************************/
#include    <3d/shader/shader.script.hh>

namespace BugEngine
{

namespace Shaders
{
class Float4;
}

class be_api(TEXT) OutlineShader : public ShaderProgramDescription
{
    friend class OutlineShaderManager;
published:
    OutlineShader(weak<Shaders::Float4> textColor,
                  weak<Shaders::Float4> backgroundColor);
    ~OutlineShader();
};

}

/*****************************************************************************/
#endif
