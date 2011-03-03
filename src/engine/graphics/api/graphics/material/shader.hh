/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_MATERIAL_SHADER_HH_
#define BE_GRAPHICS_MATERIAL_SHADER_HH_
/*****************************************************************************/
#include    <system/resource/resource.hh>
#include    <graphics/material/shaders/node.hh>

namespace BugEngine { namespace Graphics
{

class IShaderBuilder;

class be_api(GRAPHICS) Shader : public Resource
{
private:
    ref<Shaders::Node> m_root;
published:
    Shader();
    ~Shader();

    void buildSource(const IShaderBuilder& builder) const;
};

}}

/*****************************************************************************/
#endif
