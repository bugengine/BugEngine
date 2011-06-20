/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_OBJECTS_SHADERS_PARAM_SCRIPT_HH_
#define BE_GRAPHICS_OBJECTS_SHADERS_PARAM_SCRIPT_HH_
/*****************************************************************************/
#include    <graphics/objects/shaders/node.script.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{

class be_api(GRAPHICS) Param : public Node
{
published:
    const istring           name;
    const ref<const Node>   value;
published:
    Param(const istring& name, ref<const Node> value);
    ~Param();
public:
    virtual void buildSource(IShaderBuilder& stream) const override;
};

class be_api(GRAPHICS) Uniform : public Node
{
published:
    const istring           name;
published:
    Uniform(const istring& name);
    ~Uniform();
public:
    virtual void buildSource(IShaderBuilder& stream) const override;
};

}}}

/*****************************************************************************/
#endif
