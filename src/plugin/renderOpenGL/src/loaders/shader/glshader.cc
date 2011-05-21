/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>
#include    <extensions.hh>
#include    <loaders/shader/glshader.hh>
#include    <loaders/shader/glshaderbuilder.hh>
#include    <graphics/objects/shader.script.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

GLShader::GLShader(weak<const Resource> resource)
    :   IGPUResource(resource)
{
}

GLShader::~GLShader()
{
}

void GLShader::load(weak<IRenderer> renderer)
{
    weak<const Shader> shader = be_checked_cast<const Shader>(m_resource);
    GLenum shaderType = GL_VERTEX_SHADER;
    switch(shader->type)
    {
    case Shader::Vertex:    shaderType = GL_VERTEX_SHADER; break;
    case Shader::Fragment:  shaderType = GL_FRAGMENT_SHADER; break;
    case Shader::Geometry:  shaderType = GL_GEOMETRY_SHADER; break;
    default:
        be_error("Unknown shader type: %d" | shader->type);
        return;
    }

    GLShaderBuilder builder;
    shader->buildSource(builder);

    const ShaderExtensions& shaderext = be_checked_cast<Renderer>(renderer)->shaderext();
    m_shader = shaderext.glCreateShader(shaderType);
    GLint size = be_checked_numcast<GLint>(builder.textSize());
    const GLcharARB* text = (GLcharARB*)builder.text();
    shaderext.glShaderSource(m_shader, 1, &text, &size);
    shaderext.glCompileShader(m_shader);
#ifdef BE_DEBUG
    GLint errors, loglength;
    shaderext.glGetObjectParameteriv(m_shader, GL_OBJECT_COMPILE_STATUS_ARB, &errors);
    shaderext.glGetObjectParameteriv(m_shader, GL_OBJECT_INFO_LOG_LENGTH_ARB, &loglength);
    if (errors || loglength)
    {
        GLsizei maxLength = loglength, result;
        Allocator::Block<GLcharARB> log(tempArena(), loglength);
        shaderext.glGetInfoLog(m_shader, maxLength, &result, log.data());
        if (errors)
        {
            be_error(log.data());
        }
        else
        {
            be_warning(log.data());
        }
    }
#endif
}

void GLShader::unload(weak<IRenderer> renderer)
{
}

}}}
