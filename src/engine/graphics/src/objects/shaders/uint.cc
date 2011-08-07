/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#include    <graphics/stdafx.h>
#include    <graphics/objects/shaders/uint.script.hh>
#include    <graphics/tools/ishaderbuilder.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{
/* Type *************************************************************/
Uint::Uint()
{
}
Uint::~Uint()
{
}

/* Uniform **********************************************************/
UintUniform::UintUniform(const istring& name)
    :   name(name)
{
}
UintUniform::~UintUniform()
{
}
void UintUniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Uint);
}
void UintUniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
UintAttribute::UintAttribute()
{
}
UintAttribute::~UintAttribute()
{
}
void UintAttribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Uint);
}
void UintAttribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
UintVarying::UintVarying(weak<const Uint> node)
    :   node(node)
{
}
UintVarying::~UintVarying()
{
}
void UintVarying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Uint);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void UintVarying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Uint2::Uint2()
{
}
Uint2::~Uint2()
{
}

/* Uniform **********************************************************/
Uint2Uniform::Uint2Uniform(const istring& name)
    :   name(name)
{
}
Uint2Uniform::~Uint2Uniform()
{
}
void Uint2Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Uint2);
}
void Uint2Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Uint2Attribute::Uint2Attribute()
{
}
Uint2Attribute::~Uint2Attribute()
{
}
void Uint2Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Uint2);
}
void Uint2Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Uint2Varying::Uint2Varying(weak<const Uint2> node)
    :   node(node)
{
}
Uint2Varying::~Uint2Varying()
{
}
void Uint2Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Uint2);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Uint2Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Uint3::Uint3()
{
}
Uint3::~Uint3()
{
}

/* Uniform **********************************************************/
Uint3Uniform::Uint3Uniform(const istring& name)
    :   name(name)
{
}
Uint3Uniform::~Uint3Uniform()
{
}
void Uint3Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Uint3);
}
void Uint3Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Uint3Attribute::Uint3Attribute()
{
}
Uint3Attribute::~Uint3Attribute()
{
}
void Uint3Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Uint3);
}
void Uint3Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Uint3Varying::Uint3Varying(weak<const Uint3> node)
    :   node(node)
{
}
Uint3Varying::~Uint3Varying()
{
}
void Uint3Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Uint3);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Uint3Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Uint4::Uint4()
{
}
Uint4::~Uint4()
{
}

/* Uniform **********************************************************/
Uint4Uniform::Uint4Uniform(const istring& name)
    :   name(name)
{
}
Uint4Uniform::~Uint4Uniform()
{
}
void Uint4Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Uint4);
}
void Uint4Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Uint4Attribute::Uint4Attribute()
{
}
Uint4Attribute::~Uint4Attribute()
{
}
void Uint4Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Uint4);
}
void Uint4Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Uint4Varying::Uint4Varying(weak<const Uint4> node)
    :   node(node)
{
}
Uint4Varying::~Uint4Varying()
{
}
void Uint4Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Uint4);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Uint4Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

}}}
