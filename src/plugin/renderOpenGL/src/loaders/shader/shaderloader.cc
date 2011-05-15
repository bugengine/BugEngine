/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <loaders/shader/shaderloader.script.hh>
#include    <loaders/shader/glshaderbuilder.hh>
#include    <graphics/objects/shader.script.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

ShaderLoader::ShaderContext::ShaderContext(GLenum shaderType, const char *text, i64 textSize)
    :   shader(glCreateShader(shaderType))
{
    GLint size = be_checked_numcast<GLint>(textSize);
    glShaderSource(shader, 1, &text, &size);
    glCompileShader(shader);
}

ShaderLoader::ShaderContext::~ShaderContext()
{
}

ShaderLoader::ShaderLoader()
{
    attach<Shader>();
}

ShaderLoader::~ShaderLoader()
{
    detach<Shader>();
}

void* ShaderLoader::load(weak<const Resource> source) const
{
    weak<const Shader> shader = be_checked_cast<const Shader>(source);
    GLenum shaderType = GL_VERTEX_SHADER;
    switch(shader->type)
    {
    case Shader::Vertex:    shaderType = GL_VERTEX_SHADER; break;
    case Shader::Fragment:  shaderType = GL_FRAGMENT_SHADER; break;
    case Shader::Geometry:  shaderType = GL_GEOMETRY_SHADER; break;
    default:
        be_error("Unknown shader type: %d" | shader->type);
        return 0;
    }
    GLShaderBuilder builder;
    shader->buildSource(builder);
    ShaderContext* context = new ShaderContext(shaderType, builder.text(), builder.textSize());

    return context;
}

void  ShaderLoader::unload(const void* resource) const
{
    const ShaderContext* context = (const ShaderContext*) resource;
    delete context;
}

}}}
