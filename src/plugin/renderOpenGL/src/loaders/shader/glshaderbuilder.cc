/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <loaders/shader/glshaderbuilder.hh>


namespace BugEngine { namespace Graphics { namespace OpenGL
{

static const char *toString(Shaders::Type type)
{
    switch(type)
    {
    case Shaders::Type_Float:
        return "float";
    case Shaders::Type_Float2:
        return "vec2";
    case Shaders::Type_Float3:
        return "vec3";
    case Shaders::Type_Float4:
        return "vec4";
    case Shaders::Type_Float2x2:
        return "mat2";
    case Shaders::Type_Float2x3:
        return "mat2x3";
    case Shaders::Type_Float2x4:
        return "mat2x4";
    case Shaders::Type_Float3x2:
        return "mat3x2";
    case Shaders::Type_Float3x3:
        return "mat3";
    case Shaders::Type_Float3x4:
        return "mat3x4";
    case Shaders::Type_Float4x2:
        return "mat4x2";
    case Shaders::Type_Float4x3:
        return "mat4x3";
    case Shaders::Type_Float4x4:
        return "mat4";
    case Shaders::Type_Double:
        return "double";
    case Shaders::Type_Double2:
        return "dvec2";
    case Shaders::Type_Double3:
        return "dvec3";
    case Shaders::Type_Double4:
        return "dvec4";
    case Shaders::Type_Double2x2:
        return "dmat2";
    case Shaders::Type_Double2x3:
        return "dmat2x3";
    case Shaders::Type_Double2x4:
        return "dmat2x4";
    case Shaders::Type_Double3x2:
        return "dmat3x2";
    case Shaders::Type_Double3x3:
        return "dmat3";
    case Shaders::Type_Double3x4:
        return "dmat3x4";
    case Shaders::Type_Double4x2:
        return "dmat4x2";
    case Shaders::Type_Double4x3:
        return "dmat4x3";
    case Shaders::Type_Double4x4:
        return "dmat4";
    case Shaders::Type_Int:
        return "int";
    case Shaders::Type_Int2:
        return "ivec2";
    case Shaders::Type_Int3:
        return "ivec3";
    case Shaders::Type_Int4:
        return "ivec4";
    case Shaders::Type_Uint:
        return "uint";
    case Shaders::Type_Uint2:
        return "uvec2";
    case Shaders::Type_Uint3:
        return "uvec3";
    case Shaders::Type_Uint4:
        return "uvec4";
    case Shaders::Type_Bool:
        return "bool";
    case Shaders::Type_Bool2:
        return "bvec2";
    case Shaders::Type_Bool3:
        return "bvec3";
    case Shaders::Type_Bool4:
        return "bvec4";
    default:
        be_error("type %d not recognized by the GLSL shader builder" | type);
        return "mat4";
    }
}

GLShaderBuilder::GLShaderBuilder(GLenum shaderType)
    :   m_shaderType(shaderType)
{
    write("#version 140");
}

GLShaderBuilder::~GLShaderBuilder()
{
}

void GLShaderBuilder::doAddUniformDeclaration(const istring& name, Shaders::Stage /*stage*/, Shaders::Type type)
{
    write((minitl::format<>("uniform %s %s;") | toString(type) | name).c_str());
}

void GLShaderBuilder::doAddVaryingDeclaration(const istring& name, Shaders::Stage stage, Shaders::Type type)
{
    if (stage == Shaders::VertexStage)
    {
        write((minitl::format<>("out %s %s") | toString(type) | name).c_str());
    }
    else
    {
        write((minitl::format<>("in %s %s") | toString(type) | name).c_str());
    }
}

}}}
