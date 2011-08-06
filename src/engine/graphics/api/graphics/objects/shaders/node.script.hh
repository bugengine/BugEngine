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

enum Type
{
    Type_Float, Type_Float2, Type_Float3, Type_Float4,
    Type_Float2x2, Type_Float2x3, Type_Float2x4,
    Type_Float3x2, Type_Float3x3, Type_Float3x4,
    Type_Float4x2, Type_Float4x3, Type_Float4x4,

    Type_Double, Type_Double2, Type_Double3, Type_Double4,
    Type_Double2x2, Type_Double2x3, Type_Double2x4,
    Type_Double3x2, Type_Double3x3, Type_Double3x4,
    Type_Double4x2, Type_Double4x3, Type_Double4x4,

    Type_Int, Type_Int2, Type_Int3, Type_Int4,
    Type_Uint, Type_Uint2, Type_Uint3, Type_Uint4,

    Type_Bool, Type_Bool2, Type_Bool3, Type_Bool4,

    Type_Sampler1D, Type_Sampler2D, Type_Sampler3D, Type_SamplerCube
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
