/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.graphics.GL4/stdafx.h>
#include <bugengine/plugin.graphics.3d/shader/node.script.hh>
#include <bugengine/plugin.graphics.3d/shader/shader.script.hh>
#include <bugengine/plugin.graphics.GL4/glrenderer.hh>
#include <extensions.hh>
#include <loaders/shader/glshader.hh>
#include <loaders/shader/glshaderbuilder.hh>

namespace BugEngine { namespace OpenGL {

GLShaderProgram::GLShaderProgram(weak< const Resource::Description > shaderDescription,
                                 weak< const GLRenderer >            renderer)
    : IGPUResource(shaderDescription, renderer)
    , m_shaderProgram(0)
    , m_vertexShader(0)
    , m_geometryShader(0)
    , m_fragmentShader(0)
{
}

GLShaderProgram::~GLShaderProgram()
{
}

void GLShaderProgram::attach()
{
    be_assert(m_shaderProgram, "no program created");

    if(m_vertexShader)
        be_checked_cast< const GLRenderer >(m_renderer)
            ->shaderext()
            .glAttachShader(m_shaderProgram, m_vertexShader);
    if(m_geometryShader)
        be_checked_cast< const GLRenderer >(m_renderer)
            ->shaderext()
            .glAttachShader(m_shaderProgram, m_geometryShader);
    if(m_fragmentShader)
        be_checked_cast< const GLRenderer >(m_renderer)
            ->shaderext()
            .glAttachShader(m_shaderProgram, m_fragmentShader);
}

static GLenum toGLShaderStage(Shaders::Stage stage)
{
    switch(stage)
    {
    case Shaders::VertexStage: return GL_VERTEX_SHADER_ARB;
    case Shaders::GeometryStage: return GL_GEOMETRY_SHADER_EXT;
    case Shaders::FragmentStage: return GL_FRAGMENT_SHADER_ARB;
    default: be_error("Unknown shader type %d" | stage); return GL_FRAGMENT_SHADER_ARB;
    }
}

GLhandleARB GLShaderProgram::build(weak< const ShaderProgramDescription > program) const
{
    be_forceuse(program);
    toGLShaderStage(Shaders::FragmentStage);
#if 0
    //GLenum shaderType = toGLShaderStage(stage);
    GLShaderBuilder builder(shaderType);
    program->buildSource(builder);

    const ShaderExtensions& shaderext = be_checked_cast<const GLRenderer>(m_renderer)->shaderext();
    GLhandleARB shader = shaderext.glCreateShader(shaderType);
    GLint size = be_checked_numcast<GLint>(builder.textSize());
    const GLcharARB* text = (GLcharARB*)builder.text();
    shaderext.glShaderSource(shader, 1, &text, &size);
    shaderext.glCompileShader(shader);
#    ifdef BE_DEBUG
    GLint success, loglength;
    shaderext.glGetObjectParameteriv(shader, GL_COMPILE_STATUS, &success);
    shaderext.glGetObjectParameteriv(shader, GL_INFO_LOG_LENGTH, &loglength);
    if (!success || loglength)
    {
        GLsizei maxLength = loglength, result;
        minitl::Allocator::Block<GLcharARB> log(Arena::stack(), loglength);
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
#    endif
    return shader;
#else
    return 0;
#endif
}

void GLShaderProgram::load(weak< const Resource::Description > shaderDescription)
{
    weak< const ShaderProgramDescription > program
        = be_checked_cast< const ShaderProgramDescription >(shaderDescription);
    be_assert(m_shaderProgram == 0, "shader program loaded twice?");

    const ShaderExtensions& shaderext
        = be_checked_cast< const GLRenderer >(m_renderer)->shaderext();
    m_shaderProgram = shaderext.glCreateProgram();

    m_vertexShader   = build(program);
    m_geometryShader = build(program);
    m_fragmentShader = build(program);
    attach();
    shaderext.glLinkProgram(m_shaderProgram);

#ifdef BE_DEBUG
    GLint success, loglength;
    shaderext.glGetObjectParameteriv(m_shaderProgram, GL_LINK_STATUS, &success);
    shaderext.glGetObjectParameteriv(m_shaderProgram, GL_INFO_LOG_LENGTH, &loglength);
    if(!success || loglength)
    {
        GLsizei                               maxLength = loglength, result;
        minitl::Allocator::Block< GLcharARB > log(Arena::stack(), loglength);
        shaderext.glGetInfoLog(m_shaderProgram, maxLength, &result, log.data());
        if(!success)
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
    if(m_vertexShader)
    {
        be_checked_cast< const GLRenderer >(m_renderer)->shaderext().glDeleteShader(m_vertexShader);
        m_vertexShader = 0;
    }
    if(m_geometryShader)
    {
        be_checked_cast< const GLRenderer >(m_renderer)
            ->shaderext()
            .glDeleteShader(m_geometryShader);
        m_geometryShader = 0;
    }
    if(m_fragmentShader)
    {
        be_checked_cast< const GLRenderer >(m_renderer)
            ->shaderext()
            .glDeleteShader(m_fragmentShader);
        m_fragmentShader = 0;
    }
    be_checked_cast< const GLRenderer >(m_renderer)->shaderext().glDeleteProgram(m_shaderProgram);
    m_shaderProgram = 0;
}

}}  // namespace BugEngine::OpenGL
