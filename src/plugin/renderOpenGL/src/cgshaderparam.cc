/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#include    <stdafx.h>
#include    <cgshaderparam.hh>
#include    <texture.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
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

void CgShaderParam::setValue(_Texture* /*value*/)
{
    AssertNotReached();
    //cgD3D9SetTexture(m_shaderParam, checked_cast<Texture*>(value)->textureObject());
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
