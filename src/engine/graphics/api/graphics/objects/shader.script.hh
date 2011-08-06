/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_OBJECTS_SHADER_SCRIPT_HH_
#define BE_GRAPHICS_OBJECTS_SHADER_SCRIPT_HH_
/*****************************************************************************/
#include    <system/resource/resource.script.hh>
#include    <graphics/objects/shaders/node.script.hh>

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
    const ref<const Shaders::Node>   position;
    const ref<const Shaders::Node>   color;
    const ref<const Shaders::Node>   depth;
published:
    ShaderProgram(                    ref<const Shaders::Node> position,
                                      ref<const Shaders::Node> color,
                   be_tag(Optional()) ref<const Shaders::Node> depth);
    ~ShaderProgram();
public:
    void buildSource(Shaders::IShaderBuilder& builder, Shaders::Stage stage) const;
};

}}

/*****************************************************************************/
#endif
