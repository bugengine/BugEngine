/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_MATERIAL_SHADERS_METHOD_SCRIPT_HH_
#define BE_GRAPHICS_MATERIAL_SHADERS_METHOD_SCRIPT_HH_
/*****************************************************************************/
#include    <graphics/objects/shaders/node.script.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{

class be_api(GRAPHICS) Method : public Node
{
protected:
    Method();
    ~Method();
public:
    virtual void buildSource(IShaderBuilder& stream) const override;
};

}}}

/*****************************************************************************/
#endif
