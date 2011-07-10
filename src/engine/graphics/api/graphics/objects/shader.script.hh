/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_OBJECTS_SHADER_SCRIPT_HH_
#define BE_GRAPHICS_OBJECTS_SHADER_SCRIPT_HH_
/*****************************************************************************/
#include    <system/resource/resource.script.hh>
#include    <graphics/objects/shaders/node.script.hh>

namespace BugEngine { namespace Graphics
{

class IShaderBuilder;

namespace Shaders
{

class Node;
class Method;
class Param;

}

class be_api(GRAPHICS) Shader : public Resource
{
protected:
    Shader();
    ~Shader();
public:
    virtual void buildSource(IShaderBuilder& stream) const = 0;
};


be_tag(ResourceLoaders())
class be_api(GRAPHICS) VertexShader : public Shader
{
published:
    const ref<const Shaders::Node>                      position;
    const ref<const Shaders::Node>                      diffuse;
    const ref<const Shaders::Node>                      specular;
    const minitl::array< ref<const Shaders::Node>, 18 > varying;
published:
    VertexShader(                    ref<const Shaders::Node> position,
                  be_tag(Optional()) ref<const Shaders::Node> diffuse,
                  be_tag(Optional()) ref<const Shaders::Node> specular,
                                     minitl::array< ref<const Shaders::Node>, 18 > varying);
    ~VertexShader();

public:
    void buildSource(IShaderBuilder& stream) const override;
};


be_tag(ResourceLoaders())
class be_api(GRAPHICS) GeometryShader : public Shader
{
published:
    GeometryShader();
    ~GeometryShader();
public:
    void buildSource(IShaderBuilder& stream) const override;
};

be_tag(ResourceLoaders())
class be_api(GRAPHICS) FragmentShader : public Shader
{
published:
    const ref<const Shaders::Node>  color;
    const ref<const Shaders::Node>  depth;
published:
    FragmentShader(                    ref<const Shaders::Node> color,
                    be_tag(Optional()) ref<const Shaders::Node> depth);
    ~FragmentShader();

public:
    void buildSource(IShaderBuilder& stream) const override;
};


be_tag(ResourceLoaders())
class be_api(GRAPHICS) ShaderProgram : public Resource
{
published:
    const ref<const VertexShader>   vertex;
    const ref<const GeometryShader> geometry;
    const ref<const FragmentShader> fragment;
published:
    ShaderProgram( be_tag(Optional()) ref<const VertexShader> vertex,
                   be_tag(Optional()) ref<const GeometryShader> geometry,
                   be_tag(Optional()) ref<const FragmentShader> fragment);
    ~ShaderProgram();
};

}}

/*****************************************************************************/
#endif
