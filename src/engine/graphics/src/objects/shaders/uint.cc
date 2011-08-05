/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#include    <graphics/stdafx.h>
#include    <graphics/objects/shaders/uint.script.hh>
#include    <graphics/tools/ishaderbuilder.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{

Uint::Uint()
{
}

Uint::~Uint()
{
}

UintUniform::UintUniform(const istring& name)
    :   name(name)
{
}

UintUniform::~UintUniform()
{
}

void UintUniform::buildDeclarations(IShaderBuilder& stream) const
{
    stream.addUniform(this, name, Type_Uint);
}

void UintUniform::buildDefinitions(IShaderBuilder& /*stream*/) const
{
}

Uint2::Uint2()
{
}

Uint2::~Uint2()
{
}

Uint2Uniform::Uint2Uniform(const istring& name)
    :   name(name)
{
}

Uint2Uniform::~Uint2Uniform()
{
}

void Uint2Uniform::buildDeclarations(IShaderBuilder& stream) const
{
    stream.addUniform(this, name, Type_Uint2);
}

void Uint2Uniform::buildDefinitions(IShaderBuilder& /*stream*/) const
{
}

Uint3::Uint3()
{
}

Uint3::~Uint3()
{
}

Uint3Uniform::Uint3Uniform(const istring& name)
    :   name(name)
{
}

Uint3Uniform::~Uint3Uniform()
{
}

void Uint3Uniform::buildDeclarations(IShaderBuilder& stream) const
{
    stream.addUniform(this, name, Type_Uint3);
}

void Uint3Uniform::buildDefinitions(IShaderBuilder& /*stream*/) const
{
}

Uint4::Uint4()
{
}

Uint4::~Uint4()
{
}

Uint4Uniform::Uint4Uniform(const istring& name)
    :   name(name)
{
}

Uint4Uniform::~Uint4Uniform()
{
}

void Uint4Uniform::buildDeclarations(IShaderBuilder& stream) const
{
    stream.addUniform(this, name, Type_Uint4);
}

void Uint4Uniform::buildDefinitions(IShaderBuilder& /*stream*/) const
{
}

}}}
