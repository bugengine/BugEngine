/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#include    <graphics/stdafx.h>
#include    <graphics/objects/shaders/double.script.hh>
#include    <graphics/tools/ishaderbuilder.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{
/* Type *************************************************************/
Double::Double()
{
}
Double::~Double()
{
}

/* Uniform **********************************************************/
DoubleUniform::DoubleUniform(const istring& name)
    :   name(name)
{
}
DoubleUniform::~DoubleUniform()
{
}
void DoubleUniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double);
}
void DoubleUniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
DoubleAttribute::DoubleAttribute()
{
}
DoubleAttribute::~DoubleAttribute()
{
}
void DoubleAttribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double);
}
void DoubleAttribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
DoubleVarying::DoubleVarying(weak<const Double> node)
    :   node(node)
{
}
DoubleVarying::~DoubleVarying()
{
}
void DoubleVarying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void DoubleVarying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Double2::Double2()
{
}
Double2::~Double2()
{
}

/* Uniform **********************************************************/
Double2Uniform::Double2Uniform(const istring& name)
    :   name(name)
{
}
Double2Uniform::~Double2Uniform()
{
}
void Double2Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double2);
}
void Double2Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double2Attribute::Double2Attribute()
{
}
Double2Attribute::~Double2Attribute()
{
}
void Double2Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double2);
}
void Double2Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double2Varying::Double2Varying(weak<const Double2> node)
    :   node(node)
{
}
Double2Varying::~Double2Varying()
{
}
void Double2Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double2);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double2Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Double3::Double3()
{
}
Double3::~Double3()
{
}

/* Uniform **********************************************************/
Double3Uniform::Double3Uniform(const istring& name)
    :   name(name)
{
}
Double3Uniform::~Double3Uniform()
{
}
void Double3Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double3);
}
void Double3Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double3Attribute::Double3Attribute()
{
}
Double3Attribute::~Double3Attribute()
{
}
void Double3Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double3);
}
void Double3Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double3Varying::Double3Varying(weak<const Double3> node)
    :   node(node)
{
}
Double3Varying::~Double3Varying()
{
}
void Double3Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double3);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double3Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Double4::Double4()
{
}
Double4::~Double4()
{
}

/* Uniform **********************************************************/
Double4Uniform::Double4Uniform(const istring& name)
    :   name(name)
{
}
Double4Uniform::~Double4Uniform()
{
}
void Double4Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double4);
}
void Double4Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double4Attribute::Double4Attribute()
{
}
Double4Attribute::~Double4Attribute()
{
}
void Double4Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double4);
}
void Double4Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double4Varying::Double4Varying(weak<const Double4> node)
    :   node(node)
{
}
Double4Varying::~Double4Varying()
{
}
void Double4Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double4);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double4Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Double2x2::Double2x2()
{
}
Double2x2::~Double2x2()
{
}

/* Uniform **********************************************************/
Double2x2Uniform::Double2x2Uniform(const istring& name)
    :   name(name)
{
}
Double2x2Uniform::~Double2x2Uniform()
{
}
void Double2x2Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double2x2);
}
void Double2x2Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double2x2Attribute::Double2x2Attribute()
{
}
Double2x2Attribute::~Double2x2Attribute()
{
}
void Double2x2Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double2x2);
}
void Double2x2Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double2x2Varying::Double2x2Varying(weak<const Double2x2> node)
    :   node(node)
{
}
Double2x2Varying::~Double2x2Varying()
{
}
void Double2x2Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double2x2);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double2x2Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Double3x2::Double3x2()
{
}
Double3x2::~Double3x2()
{
}

/* Uniform **********************************************************/
Double3x2Uniform::Double3x2Uniform(const istring& name)
    :   name(name)
{
}
Double3x2Uniform::~Double3x2Uniform()
{
}
void Double3x2Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double3x2);
}
void Double3x2Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double3x2Attribute::Double3x2Attribute()
{
}
Double3x2Attribute::~Double3x2Attribute()
{
}
void Double3x2Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double3x2);
}
void Double3x2Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double3x2Varying::Double3x2Varying(weak<const Double3x2> node)
    :   node(node)
{
}
Double3x2Varying::~Double3x2Varying()
{
}
void Double3x2Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double3x2);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double3x2Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Double4x2::Double4x2()
{
}
Double4x2::~Double4x2()
{
}

/* Uniform **********************************************************/
Double4x2Uniform::Double4x2Uniform(const istring& name)
    :   name(name)
{
}
Double4x2Uniform::~Double4x2Uniform()
{
}
void Double4x2Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double4x2);
}
void Double4x2Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double4x2Attribute::Double4x2Attribute()
{
}
Double4x2Attribute::~Double4x2Attribute()
{
}
void Double4x2Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double4x2);
}
void Double4x2Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double4x2Varying::Double4x2Varying(weak<const Double4x2> node)
    :   node(node)
{
}
Double4x2Varying::~Double4x2Varying()
{
}
void Double4x2Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double4x2);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double4x2Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Double2x3::Double2x3()
{
}
Double2x3::~Double2x3()
{
}

/* Uniform **********************************************************/
Double2x3Uniform::Double2x3Uniform(const istring& name)
    :   name(name)
{
}
Double2x3Uniform::~Double2x3Uniform()
{
}
void Double2x3Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double2x3);
}
void Double2x3Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double2x3Attribute::Double2x3Attribute()
{
}
Double2x3Attribute::~Double2x3Attribute()
{
}
void Double2x3Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double2x3);
}
void Double2x3Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double2x3Varying::Double2x3Varying(weak<const Double2x3> node)
    :   node(node)
{
}
Double2x3Varying::~Double2x3Varying()
{
}
void Double2x3Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double2x3);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double2x3Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Double3x3::Double3x3()
{
}
Double3x3::~Double3x3()
{
}

/* Uniform **********************************************************/
Double3x3Uniform::Double3x3Uniform(const istring& name)
    :   name(name)
{
}
Double3x3Uniform::~Double3x3Uniform()
{
}
void Double3x3Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double3x3);
}
void Double3x3Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double3x3Attribute::Double3x3Attribute()
{
}
Double3x3Attribute::~Double3x3Attribute()
{
}
void Double3x3Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double3x3);
}
void Double3x3Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double3x3Varying::Double3x3Varying(weak<const Double3x3> node)
    :   node(node)
{
}
Double3x3Varying::~Double3x3Varying()
{
}
void Double3x3Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double3x3);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double3x3Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Double4x3::Double4x3()
{
}
Double4x3::~Double4x3()
{
}

/* Uniform **********************************************************/
Double4x3Uniform::Double4x3Uniform(const istring& name)
    :   name(name)
{
}
Double4x3Uniform::~Double4x3Uniform()
{
}
void Double4x3Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double4x3);
}
void Double4x3Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double4x3Attribute::Double4x3Attribute()
{
}
Double4x3Attribute::~Double4x3Attribute()
{
}
void Double4x3Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double4x3);
}
void Double4x3Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double4x3Varying::Double4x3Varying(weak<const Double4x3> node)
    :   node(node)
{
}
Double4x3Varying::~Double4x3Varying()
{
}
void Double4x3Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double4x3);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double4x3Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Double2x4::Double2x4()
{
}
Double2x4::~Double2x4()
{
}

/* Uniform **********************************************************/
Double2x4Uniform::Double2x4Uniform(const istring& name)
    :   name(name)
{
}
Double2x4Uniform::~Double2x4Uniform()
{
}
void Double2x4Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double2x4);
}
void Double2x4Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double2x4Attribute::Double2x4Attribute()
{
}
Double2x4Attribute::~Double2x4Attribute()
{
}
void Double2x4Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double2x4);
}
void Double2x4Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double2x4Varying::Double2x4Varying(weak<const Double2x4> node)
    :   node(node)
{
}
Double2x4Varying::~Double2x4Varying()
{
}
void Double2x4Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double2x4);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double2x4Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Double3x4::Double3x4()
{
}
Double3x4::~Double3x4()
{
}

/* Uniform **********************************************************/
Double3x4Uniform::Double3x4Uniform(const istring& name)
    :   name(name)
{
}
Double3x4Uniform::~Double3x4Uniform()
{
}
void Double3x4Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double3x4);
}
void Double3x4Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double3x4Attribute::Double3x4Attribute()
{
}
Double3x4Attribute::~Double3x4Attribute()
{
}
void Double3x4Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double3x4);
}
void Double3x4Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double3x4Varying::Double3x4Varying(weak<const Double3x4> node)
    :   node(node)
{
}
Double3x4Varying::~Double3x4Varying()
{
}
void Double3x4Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double3x4);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double3x4Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Double4x4::Double4x4()
{
}
Double4x4::~Double4x4()
{
}

/* Uniform **********************************************************/
Double4x4Uniform::Double4x4Uniform(const istring& name)
    :   name(name)
{
}
Double4x4Uniform::~Double4x4Uniform()
{
}
void Double4x4Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double4x4);
}
void Double4x4Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double4x4Attribute::Double4x4Attribute()
{
}
Double4x4Attribute::~Double4x4Attribute()
{
}
void Double4x4Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double4x4);
}
void Double4x4Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double4x4Varying::Double4x4Varying(weak<const Double4x4> node)
    :   node(node)
{
}
Double4x4Varying::~Double4x4Varying()
{
}
void Double4x4Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double4x4);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double4x4Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

}}}
