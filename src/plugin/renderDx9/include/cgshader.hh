/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DX9_CGSHADER_HH_
#define BE_DX9_CGSHADER_HH_
/*****************************************************************************/
#include    <graphics/material/shader.hh>

#include    <d3d9.h>
#include    <Cg/cg.h>
#include    <Cg/cgD3D9.h>

namespace BugEngine { namespace Graphics { namespace DirectX9
{

class ShaderPipeline;

class CgShader : public _Shader
{
    friend class ShaderPipeline;
private:
    CGprogram                           m_program;
    std::vector< ref<ShaderParam> >  m_params;
public:
    CgShader(CGprogram program);
    ~CgShader();

    void set() const;

    virtual ref<ShaderParam> getParam(const istring& name) const override;
private:
    void addParam(ref<ShaderParam> p);
};

}}}

/*****************************************************************************/
#endif
