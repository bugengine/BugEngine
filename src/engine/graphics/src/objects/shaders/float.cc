/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#include    <graphics/stdafx.h>
#include    <graphics/objects/shaders/float.script.hh>
#include    <graphics/tools/ishaderbuilder.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{

Float::Float()
{
}

Float::~Float()
{
}

FloatUniform::FloatUniform(const istring& name)
    :   name(name)
{
}

FloatUniform::~FloatUniform()
{
}

void FloatUniform::buildDeclarations(IShaderBuilder& stream) const
{
    stream.addUniform(this, name, Type_Float);
}

void FloatUniform::buildDefinitions(IShaderBuilder& /*stream*/) const
{
}

Float2::Float2()
{
}

Float2::~Float2()
{
}

Float2Uniform::Float2Uniform(const istring& name)
    :   name(name)
{
}

Float2Uniform::~Float2Uniform()
{
}

void Float2Uniform::buildDeclarations(IShaderBuilder& stream) const
{
    stream.addUniform(this, name, Type_Float2);
}

void Float2Uniform::buildDefinitions(IShaderBuilder& /*stream*/) const
{
}

Float3::Float3()
{
}

Float3::~Float3()
{
}

Float3Uniform::Float3Uniform(const istring& name)
    :   name(name)
{
}

Float3Uniform::~Float3Uniform()
{
}

void Float3Uniform::buildDeclarations(IShaderBuilder& stream) const
{
    stream.addUniform(this, name, Type_Float3);
}

void Float3Uniform::buildDefinitions(IShaderBuilder& /*stream*/) const
{
}

Float4::Float4()
{
}

Float4::~Float4()
{
}

Float4Uniform::Float4Uniform(const istring& name)
    :   name(name)
{
}

Float4Uniform::~Float4Uniform()
{
}

void Float4Uniform::buildDeclarations(IShaderBuilder& stream) const
{
    stream.addUniform(this, name, Type_Float4);
}

void Float4Uniform::buildDefinitions(IShaderBuilder& /*stream*/) const
{
}

Float2x2::Float2x2()
{
}

Float2x2::~Float2x2()
{
}

Float2x2Uniform::Float2x2Uniform(const istring& name)
    :   name(name)
{
}

Float2x2Uniform::~Float2x2Uniform()
{
}

void Float2x2Uniform::buildDeclarations(IShaderBuilder& stream) const
{
    stream.addUniform(this, name, Type_Float2x2);
}

void Float2x2Uniform::buildDefinitions(IShaderBuilder& /*stream*/) const
{
}

Float3x2::Float3x2()
{
}

Float3x2::~Float3x2()
{
}

Float3x2Uniform::Float3x2Uniform(const istring& name)
    :   name(name)
{
}

Float3x2Uniform::~Float3x2Uniform()
{
}

void Float3x2Uniform::buildDeclarations(IShaderBuilder& stream) const
{
    stream.addUniform(this, name, Type_Float3x2);
}

void Float3x2Uniform::buildDefinitions(IShaderBuilder& /*stream*/) const
{
}

Float4x2::Float4x2()
{
}

Float4x2::~Float4x2()
{
}

Float4x2Uniform::Float4x2Uniform(const istring& name)
    :   name(name)
{
}

Float4x2Uniform::~Float4x2Uniform()
{
}

void Float4x2Uniform::buildDeclarations(IShaderBuilder& stream) const
{
    stream.addUniform(this, name, Type_Float4x2);
}

void Float4x2Uniform::buildDefinitions(IShaderBuilder& /*stream*/) const
{
}

Float2x3::Float2x3()
{
}

Float2x3::~Float2x3()
{
}

Float2x3Uniform::Float2x3Uniform(const istring& name)
    :   name(name)
{
}

Float2x3Uniform::~Float2x3Uniform()
{
}

void Float2x3Uniform::buildDeclarations(IShaderBuilder& stream) const
{
    stream.addUniform(this, name, Type_Float2x3);
}

void Float2x3Uniform::buildDefinitions(IShaderBuilder& /*stream*/) const
{
}

Float3x3::Float3x3()
{
}

Float3x3::~Float3x3()
{
}

Float3x3Uniform::Float3x3Uniform(const istring& name)
    :   name(name)
{
}

Float3x3Uniform::~Float3x3Uniform()
{
}

void Float3x3Uniform::buildDeclarations(IShaderBuilder& stream) const
{
    stream.addUniform(this, name, Type_Float3x3);
}

void Float3x3Uniform::buildDefinitions(IShaderBuilder& /*stream*/) const
{
}

Float4x3::Float4x3()
{
}

Float4x3::~Float4x3()
{
}

Float4x3Uniform::Float4x3Uniform(const istring& name)
    :   name(name)
{
}

Float4x3Uniform::~Float4x3Uniform()
{
}

void Float4x3Uniform::buildDeclarations(IShaderBuilder& stream) const
{
    stream.addUniform(this, name, Type_Float4x3);
}

void Float4x3Uniform::buildDefinitions(IShaderBuilder& /*stream*/) const
{
}

Float2x4::Float2x4()
{
}

Float2x4::~Float2x4()
{
}

Float2x4Uniform::Float2x4Uniform(const istring& name)
    :   name(name)
{
}

Float2x4Uniform::~Float2x4Uniform()
{
}

void Float2x4Uniform::buildDeclarations(IShaderBuilder& stream) const
{
    stream.addUniform(this, name, Type_Float2x4);
}

void Float2x4Uniform::buildDefinitions(IShaderBuilder& /*stream*/) const
{
}

Float3x4::Float3x4()
{
}

Float3x4::~Float3x4()
{
}

Float3x4Uniform::Float3x4Uniform(const istring& name)
    :   name(name)
{
}

Float3x4Uniform::~Float3x4Uniform()
{
}

void Float3x4Uniform::buildDeclarations(IShaderBuilder& stream) const
{
    stream.addUniform(this, name, Type_Float3x4);
}

void Float3x4Uniform::buildDefinitions(IShaderBuilder& /*stream*/) const
{
}

Float4x4::Float4x4()
{
}

Float4x4::~Float4x4()
{
}

Float4x4Uniform::Float4x4Uniform(const istring& name)
    :   name(name)
{
}

Float4x4Uniform::~Float4x4Uniform()
{
}

void Float4x4Uniform::buildDeclarations(IShaderBuilder& stream) const
{
    stream.addUniform(this, name, Type_Float4x4);
}

void Float4x4Uniform::buildDefinitions(IShaderBuilder& /*stream*/) const
{
}

}}}
