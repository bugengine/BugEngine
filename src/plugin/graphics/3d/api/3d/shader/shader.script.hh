/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_3D_SHADER_SHADER_SCRIPT_HH_
#define BE_3D_SHADER_SHADER_SCRIPT_HH_
/*****************************************************************************/
#include    <resource/description.script.hh>

namespace BugEngine { namespace Shaders
{

class Node;
class Param;
class IShaderBuilder;

enum Stage
{
    VertexStage,
    GeometryStage,
    FragmentStage
};

}

class be_api(_3D) ShaderProgramDescription : public Resource::Description
{
    BE_NOCOPY(ShaderProgramDescription);
protected:
    ShaderProgramDescription();
    ~ShaderProgramDescription();
public:
    virtual void buildSource(Shaders::IShaderBuilder& builder, Shaders::Stage stage) const;
};

}

/*****************************************************************************/
#endif
