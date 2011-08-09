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
    ,   m_shaderProgram(0)
{
}

GLShaderProgram::~GLShaderProgram()
{
}

void GLShaderProgram::attach()
{
    be_assert(m_shaderProgram, "no program created");

    if (m_vertexShader)
        be_checked_cast<const GLRenderer>(m_renderer)->shaderext().glAttachShader(m_shaderProgram, m_vertexShader);
    //if (m_geometryShader)
    //    be_checked_cast<const GLRenderer>(m_renderer)->shaderext().glAttachShader(m_shaderProgram, m_geometryShader);
    if (m_fragmentShader)
        be_checked_cast<const GLRenderer>(m_renderer)->shaderext().glAttachShader(m_shaderProgram, m_fragmentShader);
}

static GLenum toGLShaderStage(Shaders::Stage stage)
{
    switch(stage)
    {
    case Shaders::VertexStage:
        return GL_VERTEX_SHADER_ARB;
    case Shaders::GeometryStage:
        return GL_GEOMETRY_SHADER_EXT;
    case Shaders::FragmentStage:
        return GL_FRAGMENT_SHADER_ARB;
    default:
        be_error("Unknown shader type %d" | stage);
        return GL_FRAGMENT_SHADER_ARB;
    }
}

GLhandleARB GLShaderProgram::build(weak<const ShaderProgram> program, Shaders::Stage stage) const
{
    GLenum shaderType = toGLShaderStage(stage);
    GLShaderBuilder builder(shaderType);
    program->buildSource(builder, stage);

    const ShaderExtensions& shaderext = be_checked_cast<const GLRenderer>(m_renderer)->shaderext();
    GLhandleARB shader = shaderext.glCreateShader(shaderType);
    GLint size = be_checked_numcast<GLint>(builder.textSize());
    const GLcharARB* text = (GLcharARB*)builder.text();
    shaderext.glShaderSource(shader, 1, &text, &size);
    shaderext.glCompileShader(shader);
#ifdef BE_DEBUG
    GLint success, loglength;
    shaderext.glGetObjectParameteriv(shader, GL_OBJECT_COMPILE_STATUS_ARB, &success);
    shaderext.glGetObjectParameteriv(shader, GL_OBJECT_INFO_LOG_LENGTH_ARB, &loglength);
    if (!success || loglength)
    {
        GLsizei maxLength = loglength, result;
        Allocator::Block<GLcharARB> log(tempArena(), loglength);
        shaderext.glGetInfoLog(shader, maxLength, &result, log.data());
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
    return shader;
}

void GLShaderProgram::load(weak<const Resource> resource)
{
    weak<const ShaderProgram> program = be_checked_cast<const ShaderProgram>(resource);
    be_assert(m_shaderProgram == 0, "shader program loaded twice?");

    const ShaderExtensions& shaderext = be_checked_cast<const GLRenderer>(m_renderer)->shaderext();
    m_shaderProgram = shaderext.glCreateProgram();

    m_vertexShader = build (program, Shaders::VertexStage);
    m_geometryShader = build (program, Shaders::GeometryStage);
    m_fragmentShader = build (program, Shaders::FragmentStage);
    attach();
    shaderext.glLinkProgram(m_shaderProgram);

#ifdef BE_DEBUG
    GLint success, loglength;
    shaderext.glGetObjectParameteriv(m_shaderProgram, GL_OBJECT_LINK_STATUS_ARB, &success);
    shaderext.glGetObjectParameteriv(m_shaderProgram, GL_OBJECT_INFO_LOG_LENGTH_ARB, &loglength);
    if (!success || loglength)
    {
        GLsizei maxLength = loglength, result;
        Allocator::Block<GLcharARB> log(tempArena(), loglength);
        shaderext.glGetInfoLog(m_shaderProgram, maxLength, &result, log.data());
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

void GLShaderProgram::unload()
{
    if (m_vertexShader)
    {
        be_checked_cast<const GLRenderer>(m_renderer)->shaderext().glDeleteShader(m_vertexShader);
        m_vertexShader = 0;
    }
    if (m_geometryShader)
    {
        be_checked_cast<const GLRenderer>(m_renderer)->shaderext().glDeleteShader(m_geometryShader);
        m_geometryShader = 0;
    }
    if (m_fragmentShader)
    {
        be_checked_cast<const GLRenderer>(m_renderer)->shaderext().glDeleteShader(m_fragmentShader);
        m_fragmentShader = 0;
    }
    be_checked_cast<const GLRenderer>(m_renderer)->shaderext().glDeleteProgram(m_shaderProgram);
    m_shaderProgram = 0;
}

}}}
