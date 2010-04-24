/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DX9_SHADERPIPELINE_HH_
#define BE_DX9_SHADERPIPELINE_HH_
/*****************************************************************************/
#include    <graphics/material/shaderpipeline.hh>
#include    <graphics/material/shaderparam.hh>
#include    <Cg/cg.h>
#include    <Cg/cgD3D9.h>

namespace BugEngine { namespace Graphics { namespace DirectX9
{

class Renderer;
class CgShaderParam;

class ShaderPipeline : public Graphics::ShaderPipeline
{
private:
    weak<Renderer>                          m_owner;
    CGprofile                               m_vertexProfile;
    CGprofile                               m_fragmentProfile;
    std::map< istring, ref<CgShaderParam> > m_systemParams;
public:
    ShaderPipeline(weak<Renderer> owner);
    ~ShaderPipeline();

    Shader*                 load(const ifilename& file) override;
    void                    unload(Shader* s) override;

    ShaderParam::Type       getTypeByName(const char *type) override;
    const char *            getTypeName(ShaderParam::Type type) override;

    ref<CgShaderParam>      createSystemParameter(const istring& name, ShaderParam::Type type);
private:
    bool                    isSystemParameter(const char *name);
    ref<CgShaderParam>      getSystemParameter(const char *name);
};

}}}

/*****************************************************************************/
#endif
