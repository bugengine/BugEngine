/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#include    <graphics/stdafx.h>
#include    <graphics/objects/shaders/bool.script.hh>
#include    <graphics/tools/ishaderbuilder.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{
/* Type *************************************************************/
Bool::Bool()
{
}
Bool::~Bool()
{
}

/* Uniform **********************************************************/
BoolUniform::BoolUniform(const istring& name)
    :   name(name)
{
}
BoolUniform::~BoolUniform()
{
}
void BoolUniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Bool);
}
void BoolUniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
BoolAttribute::BoolAttribute()
{
}
BoolAttribute::~BoolAttribute()
{
}
void BoolAttribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Bool);
}
void BoolAttribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
BoolVarying::BoolVarying(weak<const Bool> node)
    :   node(node)
{
}
BoolVarying::~BoolVarying()
{
}
void BoolVarying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Bool);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void BoolVarying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Bool2::Bool2()
{
}
Bool2::~Bool2()
{
}

/* Uniform **********************************************************/
Bool2Uniform::Bool2Uniform(const istring& name)
    :   name(name)
{
}
Bool2Uniform::~Bool2Uniform()
{
}
void Bool2Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Bool2);
}
void Bool2Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Bool2Attribute::Bool2Attribute()
{
}
Bool2Attribute::~Bool2Attribute()
{
}
void Bool2Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Bool2);
}
void Bool2Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Bool2Varying::Bool2Varying(weak<const Bool2> node)
    :   node(node)
{
}
Bool2Varying::~Bool2Varying()
{
}
void Bool2Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Bool2);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Bool2Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Bool3::Bool3()
{
}
Bool3::~Bool3()
{
}

/* Uniform **********************************************************/
Bool3Uniform::Bool3Uniform(const istring& name)
    :   name(name)
{
}
Bool3Uniform::~Bool3Uniform()
{
}
void Bool3Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Bool3);
}
void Bool3Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Bool3Attribute::Bool3Attribute()
{
}
Bool3Attribute::~Bool3Attribute()
{
}
void Bool3Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Bool3);
}
void Bool3Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Bool3Varying::Bool3Varying(weak<const Bool3> node)
    :   node(node)
{
}
Bool3Varying::~Bool3Varying()
{
}
void Bool3Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Bool3);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Bool3Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Bool4::Bool4()
{
}
Bool4::~Bool4()
{
}

/* Uniform **********************************************************/
Bool4Uniform::Bool4Uniform(const istring& name)
    :   name(name)
{
}
Bool4Uniform::~Bool4Uniform()
{
}
void Bool4Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Bool4);
}
void Bool4Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Bool4Attribute::Bool4Attribute()
{
}
Bool4Attribute::~Bool4Attribute()
{
}
void Bool4Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Bool4);
}
void Bool4Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Bool4Varying::Bool4Varying(weak<const Bool4> node)
    :   node(node)
{
}
Bool4Varying::~Bool4Varying()
{
}
void Bool4Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Bool4);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Bool4Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

}}}
