/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_MATERIAL_SHADERS_NODE_HH_
#define BE_GRAPHICS_MATERIAL_SHADERS_NODE_HH_
/*****************************************************************************/
#include    <system/resource/resource.hh>
#include    <core/memory/streams.hh>

namespace BugEngine { namespace Graphics
{
class IShaderBuilder;
}}

namespace BugEngine { namespace Graphics { namespace Shaders
{

class be_api(GRAPHICS) Node : public minitl::refcountable
{
public:
    Node();
    ~Node();

    virtual void buildSource(const IShaderBuilder& stream) = 0;
};

}}}

/*****************************************************************************/
#endif
