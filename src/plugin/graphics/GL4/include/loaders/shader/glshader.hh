/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_GL4_LOADERS_SHADER_GLSHADER_HH_
#define BE_GL4_LOADERS_SHADER_GLSHADER_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.GL4/stdafx.h>
#include <bugengine/plugin.graphics.3d/renderer/igpuresource.hh>
#include <bugengine/plugin.graphics.3d/shader/shader.script.hh>

namespace BugEngine { namespace OpenGL {

class GLRenderer;

class GLShaderProgram : public IGPUResource
{
private:
    GLhandleARB m_shaderProgram;
    GLhandleARB m_vertexShader;
    GLhandleARB m_geometryShader;
    GLhandleARB m_fragmentShader;

private:
    GLhandleARB build(weak< const ShaderProgramDescription > program) const;
    void        attach();

public:
    GLShaderProgram(weak< const Resource::Description > shaderDescription,
                    weak< const GLRenderer >            renderer);
    ~GLShaderProgram();

    virtual void load(weak< const Resource::Description > shaderDescription) override;
    virtual void unload() override;
};

}}  // namespace BugEngine::OpenGL

/**************************************************************************************************/
#endif
