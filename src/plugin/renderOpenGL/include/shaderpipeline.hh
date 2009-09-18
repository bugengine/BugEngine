/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_SHADERPIPELINE_HH_
#define BE_OPENGL_SHADERPIPELINE_HH_
/*****************************************************************************/
#include    <graphics/material/shaderpipeline.hh>
#include    <graphics/material/shaderparam.hh>
#include    <Cg/cg.h>
#include    <Cg/cgGL.h>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

class Renderer;
class CgShaderParam;

class ShaderPipeline : public Graphics::ShaderPipeline
{
private:
    Renderer*                                   m_owner;
    CGprofile                                   m_vertexProfile;
    CGprofile                                   m_fragmentProfile;
    std::map< istring, refptr<CgShaderParam> >  m_systemParams;
public:
    ShaderPipeline(Renderer* owner);
    ~ShaderPipeline();

    _Shader*                load(const ifilename& file) override;
    void                    unload(_Shader* s) override;

    ShaderParam::Type       getTypeByName(const char *type) override;
    const char *            getTypeName(ShaderParam::Type type) override;

    refptr<CgShaderParam>   createSystemParameter(const istring& name, ShaderParam::Type type);
private:
    bool                    isSystemParameter(const char *name);
    CgShaderParam*          getSystemParameter(const char *name);
};

}}}

/*****************************************************************************/
#endif
