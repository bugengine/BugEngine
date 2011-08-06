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

}}}
