/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_OBJECTS_SHADER_SCRIPT_HH_
#define BE_GRAPHICS_OBJECTS_SHADER_SCRIPT_HH_
/*****************************************************************************/
#include    <system/resource/resource.script.hh>

namespace BugEngine { namespace Graphics
{

namespace Shaders
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

be_tag(ResourceLoaders())
class be_api(_3D) ShaderProgram : public Resource
{
    BE_NOCOPY(ShaderProgram);
protected:
    ShaderProgram();
    ~ShaderProgram();
public:
    virtual void buildSource(Shaders::IShaderBuilder& builder, Shaders::Stage stage) const = 0;
};

}}

/*****************************************************************************/
#endif
