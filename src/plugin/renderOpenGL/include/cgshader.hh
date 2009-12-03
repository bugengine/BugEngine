/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_CGSHADER_HH_
#define BE_OPENGL_CGSHADER_HH_
/*****************************************************************************/
#include    <graphics/material/shader.hh>

#include    <Cg/cg.h>
#include    <Cg/cgGL.h>

namespace BugEngine { namespace Graphics { namespace OpenGL
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

    virtual ShaderParam* getParam(const istring& name) const override;
private:
    void addParam(ShaderParam* p);
};

}}}

/*****************************************************************************/
#endif
