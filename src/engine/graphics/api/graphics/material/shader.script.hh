/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_MATERIAL_SHADER_SCRIPT_HH_
#define BE_GRAPHICS_MATERIAL_SHADER_SCRIPT_HH_
/*****************************************************************************/
#include    <graphics/material/shaders/node.script.hh>

namespace BugEngine { namespace Graphics
{

class IShaderBuilder;

class be_api(GRAPHICS) Shader : public minitl::refcountable
{
private:
    ref<Shaders::Node> m_root;
published:
    Shader(ref<Shaders::Node> root);
    ~Shader();
public:
    void buildSource(const IShaderBuilder& builder) const;
};

}}

/*****************************************************************************/
#endif
