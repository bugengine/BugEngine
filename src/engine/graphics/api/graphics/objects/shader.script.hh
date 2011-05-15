/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_MATERIAL_SHADER_SCRIPT_HH_
#define BE_GRAPHICS_MATERIAL_SHADER_SCRIPT_HH_
/*****************************************************************************/
#include    <graphics/objects/shaders/node.script.hh>
#include    <system/resource/resource.script.hh>

namespace BugEngine { namespace Graphics
{

class IShaderBuilder;

be_tag(ResourceLoaders())
class be_api(GRAPHICS) Shader : public Resource
{
published:
    enum ShaderType
    {
        Vertex,
        Fragment,
        Geometry
    };

published:
    const ref<const Shaders::Node>  root;
    const ShaderType                type;

    Shader(ShaderType type, ref<Shaders::Node> root);
    ~Shader();
public:
    void buildSource(const IShaderBuilder& builder) const;
};

}}

/*****************************************************************************/
#endif
