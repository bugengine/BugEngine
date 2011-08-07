/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_OBJECTS_SHADER_SCRIPT_HH_
#define BE_GRAPHICS_OBJECTS_SHADER_SCRIPT_HH_
/*****************************************************************************/
#include    <system/resource/resource.script.hh>
#include    <graphics/objects/shaders/node.script.hh>
#include    <graphics/objects/shaders/float.script.hh>

namespace BugEngine { namespace Graphics
{

namespace Shaders
{

class Node;
class Method;
class Param;
class IShaderBuilder;

}


be_tag(ResourceLoaders())
class be_api(GRAPHICS) ShaderProgram : public Resource
{
    BE_NOCOPY(ShaderProgram);
published:
    const ref<const Shaders::Float4> position;
    const ref<const Shaders::Float4> color;
    const ref<const Shaders::Float>  depth;
published:
    ShaderProgram(                    ref<const Shaders::Float4> position,
                                      ref<const Shaders::Float4> color,
                   be_tag(Optional()) ref<const Shaders::Float>  depth);
    ~ShaderProgram();
public:
    void buildSource(Shaders::IShaderBuilder& builder, Shaders::Stage stage) const;
};

}}

/*****************************************************************************/
#endif
