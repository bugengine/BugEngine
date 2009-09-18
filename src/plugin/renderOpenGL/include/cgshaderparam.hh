/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_CGSHADERPARAM_HH_
#define BE_OPENGL_CGSHADERPARAM_HH_
/*****************************************************************************/
#include    <graphics/material/shaderparam.hh>

#include    <Cg/cg.h>
#include    <Cg/cgGL.h>

namespace BugEngine { namespace Graphics { namespace OpenGL
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
