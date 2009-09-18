/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DX9_CGSHADERPARAM_HH_
#define BE_DX9_CGSHADERPARAM_HH_
/*****************************************************************************/
#include    <graphics/material/shaderparam.hh>

#include    <d3d9.h>
#include    <Cg/cg.h>
#include    <Cg/cgD3D9.h>

namespace BugEngine { namespace Graphics { namespace DirectX9
{

class ShaderPipeline;

class CgShaderParam : public ShaderParam
{
    friend class ShaderPipeline;
private:
    CGparameter     m_shaderParam;
public:
    CgShaderParam(CGparameter param);
    ~CgShaderParam();

    virtual void setValue(float4 value) override;
    virtual void setValue(_Texture* value) override;
    virtual Type type() const override;

    virtual const char *name() const override;
};

}}}

/*****************************************************************************/
#endif
