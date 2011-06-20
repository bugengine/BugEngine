/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_MATERIAL_SHADERS_METHOD_SCRIPT_HH_
#define BE_GRAPHICS_MATERIAL_SHADERS_METHOD_SCRIPT_HH_
/*****************************************************************************/
#include    <graphics/objects/shaders/node.script.hh>
#include    <graphics/objects/shaders/param.script.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{

class Param;

class be_api(GRAPHICS) Method : public Node
{
published:
    const minitl::vector< ref<Param> > output;
    Method(minitl::vector< ref<Param> > output);
    ~Method();
public:
    virtual void buildSource(IShaderBuilder& stream) const override;
};

}}}

/*****************************************************************************/
#endif
