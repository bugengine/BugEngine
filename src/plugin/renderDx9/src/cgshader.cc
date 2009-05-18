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
#include    <cgshader.hh>
#include    <renderer.hh>
#include    <core/memory/streams.hh>
#include    <system/filesystem.hh>
#include    <graphics/material/shaderparam.hh>

namespace BugEngine { namespace Graphics { namespace DirectX9
{

CgShader::CgShader(CGprogram program)
:   _Shader()
,   m_program(program)
{
}

CgShader::~CgShader()
{
    cgDestroyProgram(m_program);
}

void CgShader::set() const
{
    cgD3D9BindProgram(m_program);
}

void CgShader::addParam(ShaderParam* p)
{
    m_params.push_back(p);
}

ShaderParam* CgShader::getParam(const BugEngine::istring &name) const
{
    const char *str = name.c_str();
    for(std::vector< refptr<ShaderParam> >::const_iterator it = m_params.begin(); it != m_params.end(); ++it)
    {
        if(strcmp((*it)->name(), str) == 0)
            return it->get();
    }
    return 0;
}

}}}
