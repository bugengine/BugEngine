/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <cgshaderparam.hh>
#include    <texture.hh>

namespace BugEngine { namespace Graphics { namespace DirectX9
{

CgShaderParam::CgShaderParam(CGparameter param)
    :   m_shaderParam(param)
{
}

CgShaderParam::~CgShaderParam()
{
}

void CgShaderParam::setValue(BugEngine::float4 value)
{
    cgSetParameterValuefr(m_shaderParam, 4, &value[0]);
}

void CgShaderParam::setValue(_Texture* value)
{
    cgD3D9SetTexture(m_shaderParam, checked_cast<Texture*>(value)->textureObject());
}

ShaderParam::Type CgShaderParam::type() const
{
    return (int) cgGetParameterType(m_shaderParam);
}

const char* CgShaderParam::name() const
{
    return cgGetParameterName(m_shaderParam);
}

}}}
