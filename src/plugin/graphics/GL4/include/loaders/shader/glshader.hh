/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_GL4_LOADERS_SHADER_GLSHADER_HH_
#define BE_GL4_LOADERS_SHADER_GLSHADER_HH_
/*****************************************************************************/
#include    <3d/renderer/igpuresource.hh>
#include    <3d/shader/shader.script.hh>

namespace BugEngine { namespace OpenGL
{

class GLRenderer;

class GLShaderProgram : public IGPUResource
{
private:
    GLhandleARB m_shaderProgram;
    GLhandleARB m_vertexShader;
    GLhandleARB m_geometryShader;
    GLhandleARB m_fragmentShader;
private:
    GLhandleARB build(weak<const ShaderProgram> program, Shaders::Stage stage) const;
    void attach();
public:
    GLShaderProgram(weak<const Resource> resource, weak<const GLRenderer> renderer);
    ~GLShaderProgram();

    virtual void load(weak<const Resource> resource) override;
    virtual void unload() override;
};

}}

/*****************************************************************************/
#endif
