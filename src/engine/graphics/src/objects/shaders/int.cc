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

}}}
