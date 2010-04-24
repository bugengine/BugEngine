/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_MATERIAL_SHADERPARAM_HH_
#define BE_GRAPHICS_MATERIAL_SHADERPARAM_HH_
/*****************************************************************************/
#include    <minitl/ptr/refcountable.hh>
#include    <graphics/material/texture.hh>

namespace BugEngine { namespace Graphics
{

class be_api(GRAPHICS) ShaderParam : public minitl::refcountable
{
public:
    typedef int Type;
public:
    ShaderParam();
    ~ShaderParam();

    virtual void setValue(float4 value) = 0;
    virtual void setValue(Texture* value) = 0;
    virtual Type type() const = 0;

    virtual const char *name() const = 0;
};

}}

/*****************************************************************************/
#endif
