/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#include    <graphics/stdafx.h>
#include    <graphics/objects/shaders/int.script.hh>
#include    <graphics/tools/ishaderbuilder.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{
/* Type *************************************************************/
Int::Int()
{
}
Int::~Int()
{
}

/* Uniform **********************************************************/
IntUniform::IntUniform(const istring& name)
    :   name(name)
{
}
IntUniform::~IntUniform()
{
}
void IntUniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Int);
}
void IntUniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
IntAttribute::IntAttribute()
{
}
IntAttribute::~IntAttribute()
{
}
void IntAttribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Int);
}
void IntAttribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
IntVarying::IntVarying(weak<const Int> node)
    :   node(node)
{
}
IntVarying::~IntVarying()
{
}
void IntVarying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Int);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void IntVarying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Int2::Int2()
{
}
Int2::~Int2()
{
}

/* Uniform **********************************************************/
Int2Uniform::Int2Uniform(const istring& name)
    :   name(name)
{
}
Int2Uniform::~Int2Uniform()
{
}
void Int2Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Int2);
}
void Int2Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Int2Attribute::Int2Attribute()
{
}
Int2Attribute::~Int2Attribute()
{
}
void Int2Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Int2);
}
void Int2Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Int2Varying::Int2Varying(weak<const Int2> node)
    :   node(node)
{
}
Int2Varying::~Int2Varying()
{
}
void Int2Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Int2);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Int2Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Int3::Int3()
{
}
Int3::~Int3()
{
}

/* Uniform **********************************************************/
Int3Uniform::Int3Uniform(const istring& name)
    :   name(name)
{
}
Int3Uniform::~Int3Uniform()
{
}
void Int3Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Int3);
}
void Int3Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Int3Attribute::Int3Attribute()
{
}
Int3Attribute::~Int3Attribute()
{
}
void Int3Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Int3);
}
void Int3Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Int3Varying::Int3Varying(weak<const Int3> node)
    :   node(node)
{
}
Int3Varying::~Int3Varying()
{
}
void Int3Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Int3);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Int3Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Type *************************************************************/
Int4::Int4()
{
}
Int4::~Int4()
{
}

/* Uniform **********************************************************/
Int4Uniform::Int4Uniform(const istring& name)
    :   name(name)
{
}
Int4Uniform::~Int4Uniform()
{
}
void Int4Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Int4);
}
void Int4Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Int4Attribute::Int4Attribute()
{
}
Int4Attribute::~Int4Attribute()
{
}
void Int4Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Int4);
}
void Int4Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Int4Varying::Int4Varying(weak<const Int4> node)
    :   node(node)
{
}
Int4Varying::~Int4Varying()
{
}
void Int4Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Int4);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Int4Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

}}}
