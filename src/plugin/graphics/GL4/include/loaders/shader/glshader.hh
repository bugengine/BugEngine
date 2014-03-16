/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_GL4_LOADERS_SHADER_GLSHADER_HH_
#define BE_GL4_LOADERS_SHADER_GLSHADER_HH_
/**************************************************************************************************/
#include    <GL4/stdafx.h>
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
    GLhandleARB build(weak<const ShaderProgramDescription> program) const;
    void attach();
public:
    GLShaderProgram(weak<const Resource::Description> shaderDescription, weak<const GLRenderer> renderer);
    ~GLShaderProgram();

    virtual void load(weak<const Resource::Description> shaderDescription) override;
    virtual void unload() override;
};

}}

/**************************************************************************************************/
#endif
