/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#include    <graphics/stdafx.h>
#include    <graphics/objects/shaders/float.script.hh>
#include    <graphics/tools/ishaderbuilder.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{
/* Type *************************************************************/
Float::Float()
{
}
Float::~Float()
{
}

/* Uniform **********************************************************/
FloatUniform::FloatUniform(const istring& name)
    :   name(name)
{
}
FloatUniform::~FloatUniform()
{
}
void FloatUniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Float);
}
void FloatUniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
FloatAttribute::FloatAttribute()
{
}
FloatAttribute::~FloatAttribute()
{
}
void FloatAttribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Float);
}
void FloatAttribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
FloatVarying::FloatVarying(weak<const Float> node)
    :   node(node)
{
}
FloatVarying::~FloatVarying()
{
}
void FloatVarying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Float);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void FloatVarying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Float2::Float2()
{
}
Float2::~Float2()
{
}

/* Uniform **********************************************************/
Float2Uniform::Float2Uniform(const istring& name)
    :   name(name)
{
}
Float2Uniform::~Float2Uniform()
{
}
void Float2Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Float2);
}
void Float2Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float2Attribute::Float2Attribute()
{
}
Float2Attribute::~Float2Attribute()
{
}
void Float2Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Float2);
}
void Float2Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float2Varying::Float2Varying(weak<const Float2> node)
    :   node(node)
{
}
Float2Varying::~Float2Varying()
{
}
void Float2Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Float2);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Float2Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Float3::Float3()
{
}
Float3::~Float3()
{
}

/* Uniform **********************************************************/
Float3Uniform::Float3Uniform(const istring& name)
    :   name(name)
{
}
Float3Uniform::~Float3Uniform()
{
}
void Float3Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Float3);
}
void Float3Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float3Attribute::Float3Attribute()
{
}
Float3Attribute::~Float3Attribute()
{
}
void Float3Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Float3);
}
void Float3Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float3Varying::Float3Varying(weak<const Float3> node)
    :   node(node)
{
}
Float3Varying::~Float3Varying()
{
}
void Float3Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Float3);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Float3Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Float4::Float4()
{
}
Float4::~Float4()
{
}

/* Uniform **********************************************************/
Float4Uniform::Float4Uniform(const istring& name)
    :   name(name)
{
}
Float4Uniform::~Float4Uniform()
{
}
void Float4Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Float4);
}
void Float4Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float4Attribute::Float4Attribute()
{
}
Float4Attribute::~Float4Attribute()
{
}
void Float4Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Float4);
}
void Float4Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float4Varying::Float4Varying(weak<const Float4> node)
    :   node(node)
{
}
Float4Varying::~Float4Varying()
{
}
void Float4Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Float4);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Float4Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Float2x2::Float2x2()
{
}
Float2x2::~Float2x2()
{
}

/* Uniform **********************************************************/
Float2x2Uniform::Float2x2Uniform(const istring& name)
    :   name(name)
{
}
Float2x2Uniform::~Float2x2Uniform()
{
}
void Float2x2Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Float2x2);
}
void Float2x2Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float2x2Attribute::Float2x2Attribute()
{
}
Float2x2Attribute::~Float2x2Attribute()
{
}
void Float2x2Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Float2x2);
}
void Float2x2Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float2x2Varying::Float2x2Varying(weak<const Float2x2> node)
    :   node(node)
{
}
Float2x2Varying::~Float2x2Varying()
{
}
void Float2x2Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Float2x2);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Float2x2Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Float3x2::Float3x2()
{
}
Float3x2::~Float3x2()
{
}

/* Uniform **********************************************************/
Float3x2Uniform::Float3x2Uniform(const istring& name)
    :   name(name)
{
}
Float3x2Uniform::~Float3x2Uniform()
{
}
void Float3x2Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Float3x2);
}
void Float3x2Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float3x2Attribute::Float3x2Attribute()
{
}
Float3x2Attribute::~Float3x2Attribute()
{
}
void Float3x2Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Float3x2);
}
void Float3x2Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float3x2Varying::Float3x2Varying(weak<const Float3x2> node)
    :   node(node)
{
}
Float3x2Varying::~Float3x2Varying()
{
}
void Float3x2Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Float3x2);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Float3x2Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Float4x2::Float4x2()
{
}
Float4x2::~Float4x2()
{
}

/* Uniform **********************************************************/
Float4x2Uniform::Float4x2Uniform(const istring& name)
    :   name(name)
{
}
Float4x2Uniform::~Float4x2Uniform()
{
}
void Float4x2Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Float4x2);
}
void Float4x2Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float4x2Attribute::Float4x2Attribute()
{
}
Float4x2Attribute::~Float4x2Attribute()
{
}
void Float4x2Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Float4x2);
}
void Float4x2Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float4x2Varying::Float4x2Varying(weak<const Float4x2> node)
    :   node(node)
{
}
Float4x2Varying::~Float4x2Varying()
{
}
void Float4x2Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Float4x2);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Float4x2Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Float2x3::Float2x3()
{
}
Float2x3::~Float2x3()
{
}

/* Uniform **********************************************************/
Float2x3Uniform::Float2x3Uniform(const istring& name)
    :   name(name)
{
}
Float2x3Uniform::~Float2x3Uniform()
{
}
void Float2x3Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Float2x3);
}
void Float2x3Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float2x3Attribute::Float2x3Attribute()
{
}
Float2x3Attribute::~Float2x3Attribute()
{
}
void Float2x3Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Float2x3);
}
void Float2x3Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float2x3Varying::Float2x3Varying(weak<const Float2x3> node)
    :   node(node)
{
}
Float2x3Varying::~Float2x3Varying()
{
}
void Float2x3Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Float2x3);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Float2x3Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Float3x3::Float3x3()
{
}
Float3x3::~Float3x3()
{
}

/* Uniform **********************************************************/
Float3x3Uniform::Float3x3Uniform(const istring& name)
    :   name(name)
{
}
Float3x3Uniform::~Float3x3Uniform()
{
}
void Float3x3Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Float3x3);
}
void Float3x3Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float3x3Attribute::Float3x3Attribute()
{
}
Float3x3Attribute::~Float3x3Attribute()
{
}
void Float3x3Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Float3x3);
}
void Float3x3Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float3x3Varying::Float3x3Varying(weak<const Float3x3> node)
    :   node(node)
{
}
Float3x3Varying::~Float3x3Varying()
{
}
void Float3x3Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Float3x3);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Float3x3Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Float4x3::Float4x3()
{
}
Float4x3::~Float4x3()
{
}

/* Uniform **********************************************************/
Float4x3Uniform::Float4x3Uniform(const istring& name)
    :   name(name)
{
}
Float4x3Uniform::~Float4x3Uniform()
{
}
void Float4x3Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Float4x3);
}
void Float4x3Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float4x3Attribute::Float4x3Attribute()
{
}
Float4x3Attribute::~Float4x3Attribute()
{
}
void Float4x3Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Float4x3);
}
void Float4x3Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float4x3Varying::Float4x3Varying(weak<const Float4x3> node)
    :   node(node)
{
}
Float4x3Varying::~Float4x3Varying()
{
}
void Float4x3Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Float4x3);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Float4x3Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Float2x4::Float2x4()
{
}
Float2x4::~Float2x4()
{
}

/* Uniform **********************************************************/
Float2x4Uniform::Float2x4Uniform(const istring& name)
    :   name(name)
{
}
Float2x4Uniform::~Float2x4Uniform()
{
}
void Float2x4Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Float2x4);
}
void Float2x4Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float2x4Attribute::Float2x4Attribute()
{
}
Float2x4Attribute::~Float2x4Attribute()
{
}
void Float2x4Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Float2x4);
}
void Float2x4Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float2x4Varying::Float2x4Varying(weak<const Float2x4> node)
    :   node(node)
{
}
Float2x4Varying::~Float2x4Varying()
{
}
void Float2x4Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Float2x4);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Float2x4Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Float3x4::Float3x4()
{
}
Float3x4::~Float3x4()
{
}

/* Uniform **********************************************************/
Float3x4Uniform::Float3x4Uniform(const istring& name)
    :   name(name)
{
}
Float3x4Uniform::~Float3x4Uniform()
{
}
void Float3x4Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Float3x4);
}
void Float3x4Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float3x4Attribute::Float3x4Attribute()
{
}
Float3x4Attribute::~Float3x4Attribute()
{
}
void Float3x4Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Float3x4);
}
void Float3x4Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float3x4Varying::Float3x4Varying(weak<const Float3x4> node)
    :   node(node)
{
}
Float3x4Varying::~Float3x4Varying()
{
}
void Float3x4Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Float3x4);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Float3x4Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Float4x4::Float4x4()
{
}
Float4x4::~Float4x4()
{
}

/* Uniform **********************************************************/
Float4x4Uniform::Float4x4Uniform(const istring& name)
    :   name(name)
{
}
Float4x4Uniform::~Float4x4Uniform()
{
}
void Float4x4Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Float4x4);
}
void Float4x4Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float4x4Attribute::Float4x4Attribute()
{
}
Float4x4Attribute::~Float4x4Attribute()
{
}
void Float4x4Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Float4x4);
}
void Float4x4Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float4x4Varying::Float4x4Varying(weak<const Float4x4> node)
    :   node(node)
{
}
Float4x4Varying::~Float4x4Varying()
{
}
void Float4x4Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Float4x4);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Float4x4Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

}}}
