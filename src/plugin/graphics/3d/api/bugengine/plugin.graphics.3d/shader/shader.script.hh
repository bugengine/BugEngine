/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_3D_SHADER_SHADER_SCRIPT_HH_
#define BE_3D_SHADER_SHADER_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.3d/stdafx.h>
#include <bugengine/resource/description.script.hh>

namespace BugEngine {
namespace Shaders {

class Node;
class IShaderBuilder;
class Output;

enum Stage
{
    VertexStage,
    GeometryStage,
    TesselationControlStage,
    TessalationEvaluationStage,
    FragmentStage
};

}  // namespace Shaders

class be_api(3D) ShaderProgramDescription : public Resource::Description
{
    BE_NOCOPY(ShaderProgramDescription);

private:
    minitl::vector< ref< Shaders::Output > > m_outputs;

protected:
    ShaderProgramDescription(minitl::vector< ref< Shaders::Output > > outputs);
    ~ShaderProgramDescription();

public:
    virtual void buildSource(Shaders::IShaderBuilder & builder) const;
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
