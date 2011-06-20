/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <glrenderer.hh>
#include    <extensions.hh>
#include    <loaders/shader/glshader.hh>
#include    <loaders/shader/glshaderbuilder.hh>
#include    <graphics/objects/shader.script.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

GLShaderProgram::GLShaderProgram(weak<const Resource> resource, weak<GLRenderer> renderer)
    :   IGPUResource(resource, renderer)
{
}

GLShaderProgram::~GLShaderProgram()
{
}

void GLShaderProgram::load(weak<const Resource> /*resource*/)
{
}

void GLShaderProgram::unload()
{
}



GLShader::GLShader(weak<const VertexShader> resource, weak<GLRenderer> renderer)
    :   IGPUResource(resource, renderer)
    ,   m_shaderType(GL_VERTEX_SHADER)
    ,   m_shader(0)
{
}

GLShader::GLShader(weak<const GeometryShader> resource, weak<GLRenderer> renderer)
    :   IGPUResource(resource, renderer)
    ,   m_shaderType(GL_GEOMETRY_SHADER)
    ,   m_shader(0)
{
}

GLShader::GLShader(weak<const FragmentShader> resource, weak<GLRenderer> renderer)
    :   IGPUResource(resource, renderer)
    ,   m_shaderType(GL_FRAGMENT_SHADER)
    ,   m_shader(0)
{
}

GLShader::~GLShader()
{
}

void GLShader::load(weak<const Resource> resource)
{
    weak<const Shader> shader = be_checked_cast<const Shader>(resource);

    GLShaderBuilder builder;
    shader->buildSource(builder);

    const ShaderExtensions& shaderext = be_checked_cast<const GLRenderer>(m_renderer)->shaderext();
    m_shader = shaderext.glCreateShader(m_shaderType);
    GLint size = be_checked_numcast<GLint>(builder.textSize());
    const GLcharARB* text = (GLcharARB*)builder.text();
    shaderext.glShaderSource(m_shader, 1, &text, &size);
    shaderext.glCompileShader(m_shader);
#ifdef BE_DEBUG
    GLint success, loglength;
    shaderext.glGetObjectParameteriv(m_shader, GL_OBJECT_COMPILE_STATUS_ARB, &success);
    shaderext.glGetObjectParameteriv(m_shader, GL_OBJECT_INFO_LOG_LENGTH_ARB, &loglength);
    if (!success || loglength)
    {
        GLsizei maxLength = loglength, result;
        Allocator::Block<GLcharARB> log(tempArena(), loglength);
        shaderext.glGetInfoLog(m_shader, maxLength, &result, log.data());
        if (!success)
        {
            be_error(log.data());
        }
        else
        {
            be_info(log.data());
        }
    }
#endif
}

void GLShader::unload()
{
}

}}}
