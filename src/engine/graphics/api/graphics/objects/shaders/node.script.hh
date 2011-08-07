/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_OBJECTS_SHADERS_NODE_SCRIPT_HH_
#define BE_GRAPHICS_OBJECTS_SHADERS_NODE_SCRIPT_HH_
/*****************************************************************************/
#include    <core/memory/streams.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{

class IShaderBuilder;
enum Stage
{
    VertexStage,
    GeometryStage,
    FragmentStage
};

class be_api(GRAPHICS) Node : public minitl::refcountable
{
    BE_NOCOPY(Node);
public:

protected:
    Node();
    ~Node();
public:
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const = 0;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const = 0;
};

}}}

/*****************************************************************************/
#endif
