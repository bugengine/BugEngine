/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_OBJECTS_SHADERS_NODE_SCRIPT_HH_
#define BE_GRAPHICS_OBJECTS_SHADERS_NODE_SCRIPT_HH_
/*****************************************************************************/
#include    <core/memory/streams.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{

class IShaderBuilder;

enum Scope
{
    uniform,
    varying,
    local
};

enum Semantic
{
    POSITION,
    COLOR0,
    COLOR1,
    DEPTH,
    TEXCOORD0,
    TEXCOORD1,
    TEXCOORD2,
    TEXCOORD3,
    TEXCOORD4,
    TEXCOORD5,
    TEXCOORD6,
    TEXCOORD7
};

enum Type
{
    be_tag(Category("float")) Float,
    be_tag(Category("float")) Float2,
    be_tag(Category("float")) Float3,
    be_tag(Category("float")) Float4,
    be_tag(Category("float")) Float2x2,
    be_tag(Category("float")) Float2x3,
    be_tag(Category("float")) Float2x4,
    be_tag(Category("float")) Float3x2,
    be_tag(Category("float")) Float3x3,
    be_tag(Category("float")) Float3x4,
    be_tag(Category("float")) Float4x2,
    be_tag(Category("float")) Float4x3,
    be_tag(Category("float")) Float4x4,

    be_tag(Category("int")) Int,
    be_tag(Category("int")) Int2,
    be_tag(Category("int")) Int3,
    be_tag(Category("int")) Int4,
    be_tag(Category("int")) Int2x2,
    be_tag(Category("int")) Int2x3,
    be_tag(Category("int")) Int2x4,
    be_tag(Category("int")) Int3x2,
    be_tag(Category("int")) Int3x3,
    be_tag(Category("int")) Int3x4,
    be_tag(Category("int")) Int4x2,
    be_tag(Category("int")) Int4x3,
    be_tag(Category("int")) Int4x4,

    be_tag(Category("uint")) Uint,
    be_tag(Category("uint")) Uint2,
    be_tag(Category("uint")) Uint3,
    be_tag(Category("uint")) Uint4,
    be_tag(Category("uint")) Uint2x2,
    be_tag(Category("uint")) Uint2x3,
    be_tag(Category("uint")) Uint2x4,
    be_tag(Category("uint")) Uint3x2,
    be_tag(Category("uint")) Uint3x3,
    be_tag(Category("uint")) Uint3x4,
    be_tag(Category("uint")) Uint4x2,
    be_tag(Category("uint")) Uint4x3,
    be_tag(Category("uint")) Uint4x4,

    be_tag(Category("half")) Half,
    be_tag(Category("half")) Half2,
    be_tag(Category("half")) Half3,
    be_tag(Category("half")) Half4,
    be_tag(Category("half")) Half2x2,
    be_tag(Category("half")) Half2x3,
    be_tag(Category("half")) Half2x4,
    be_tag(Category("half")) Half3x2,
    be_tag(Category("half")) Half3x3,
    be_tag(Category("half")) Half3x4,
    be_tag(Category("half")) Half4x2,
    be_tag(Category("half")) Half4x3,
    be_tag(Category("half")) Half4x4,

    be_tag(Category("double")) Double,
    be_tag(Category("double")) Double2,
    be_tag(Category("double")) Double3,
    be_tag(Category("double")) Double4,
    be_tag(Category("double")) Double2x2,
    be_tag(Category("double")) Double2x3,
    be_tag(Category("double")) Double2x4,
    be_tag(Category("double")) Double3x2,
    be_tag(Category("double")) Double3x3,
    be_tag(Category("double")) Double3x4,
    be_tag(Category("double")) Double4x2,
    be_tag(Category("double")) Double4x3,
    be_tag(Category("double")) Double4x4,

    be_tag(Category("bool")) Bool,
    be_tag(Category("bool")) Bool2,
    be_tag(Category("bool")) Bool3,
    be_tag(Category("bool")) Bool4,
    be_tag(Category("bool")) Bool2x2,
    be_tag(Category("bool")) Bool2x3,
    be_tag(Category("bool")) Bool2x4,
    be_tag(Category("bool")) Bool3x2,
    be_tag(Category("bool")) Bool3x3,
    be_tag(Category("bool")) Bool3x4,
    be_tag(Category("bool")) Bool4x2,
    be_tag(Category("bool")) Bool4x3,
    be_tag(Category("bool")) Bool4x4,

    be_tag(Category("sampler")) Sampler1D,
    be_tag(Category("sampler")) Sampler2D,
    be_tag(Category("sampler")) Sampler3D,
    be_tag(Category("sampler")) SamplerCube,

    be_tag(Category("sampler")) ShadowSampler1D,
    be_tag(Category("sampler")) ShadowSampler2D,
    be_tag(Category("sampler")) ShadowSampler3D,
    be_tag(Category("sampler")) ShadowSamplerCube
};

class be_api(GRAPHICS) Node : public minitl::refcountable
{
    BE_NOCOPY(Node);
public:

protected:
    Node();
    ~Node();
public:
    virtual void buildDeclarations(IShaderBuilder& stream) const = 0;
    virtual void buildDefinitions(IShaderBuilder& stream) const = 0;
};

}}}

/*****************************************************************************/
#endif
