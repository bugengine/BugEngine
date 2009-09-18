/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

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
