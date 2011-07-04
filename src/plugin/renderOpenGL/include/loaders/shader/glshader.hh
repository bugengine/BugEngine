/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_LOADERS_SHADER_GLSHADER_HH_
#define BE_OPENGL_LOADERS_SHADER_GLSHADER_HH_
/*****************************************************************************/
#include    <graphics/renderer/igpuresource.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

class GLRenderer;

class GLShaderProgram : public IGPUResource
{
private:
    GLhandleARB m_shaderProgram;
private:
    void attach(weak<const Shader> shader);
public:
    GLShaderProgram(weak<const Resource> resource, weak<GLRenderer> renderer);
    ~GLShaderProgram();

    virtual void load(weak<const Resource> resource) override;
    virtual void unload() override;
};

class GLShader : public IGPUResource
{
    friend class GLShaderProgram;
private:
    GLenum      m_shaderType;
    GLhandleARB m_shader;
public:
    GLShader(weak<const VertexShader> resource, weak<GLRenderer> renderer);
    GLShader(weak<const GeometryShader> resource, weak<GLRenderer> renderer);
    GLShader(weak<const FragmentShader> resource, weak<GLRenderer> renderer);
    ~GLShader();

    virtual void load(weak<const Resource> resource) override;
    virtual void unload() override;
};

}}}

/*****************************************************************************/
#endif
