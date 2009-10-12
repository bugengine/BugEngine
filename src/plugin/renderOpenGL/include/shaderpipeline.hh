/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_SHADERPIPELINE_HH_
#define BE_OPENGL_SHADERPIPELINE_HH_
/*****************************************************************************/
#include    <graphics/material/shaderpipeline.hh>
#include    <graphics/material/shaderparam.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

class Renderer;

class ShaderPipeline : public Graphics::ShaderPipeline
{
private:
    Renderer*                                   m_owner;
public:
    ShaderPipeline(Renderer* owner);
    ~ShaderPipeline();

    _Shader*                load(const ifilename& file) override;
    void                    unload(_Shader* s) override;

    ShaderParam::Type       getTypeByName(const char *type) override;
    const char *            getTypeName(ShaderParam::Type type) override;
};

}}}

/*****************************************************************************/
#endif
