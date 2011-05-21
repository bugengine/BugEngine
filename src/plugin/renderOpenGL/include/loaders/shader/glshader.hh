/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_LOADERS_SHADER_GLSHADER_HH_
#define BE_OPENGL_LOADERS_SHADER_GLSHADER_HH_
/*****************************************************************************/
#include    <graphics/renderer/igpuresource.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

class GLShader : public IGPUResource
{
private:
    GLhandleARB m_shader;
public:
    GLShader(weak<const Resource> resource);
    ~GLShader();

    virtual void load(weak<IRenderer> renderer) override;
    virtual void unload(weak<IRenderer> renderer) override;
};

}}}

/*****************************************************************************/
#endif
