/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <shaderpipeline.hh>
#include    <renderer.hh>
#include    <cgshader.hh>
#include    <cgshaderparam.hh>

#include    <core/memory/streams.hh>
#include    <system/filesystem.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

ShaderPipeline::ShaderPipeline(Renderer* owner)
    :   m_owner(owner)
    ,   m_vertexProfile(cgGetProfile("vs_2_x"))
    ,   m_fragmentProfile(cgGetProfile("ps_2_x"))
{
}

ShaderPipeline::~ShaderPipeline()
{
}

_Shader* ShaderPipeline::load(const ifilename& filename)
{
    refptr<AbstractMemoryStream> file = FileSystem::instance()->open(filename, eReadOnly);
    const char *ext = filename[filename.size()-1].c_str();
    size_t extpos = strlen(ext) - 5;
    CGprofile p = m_vertexProfile;
    if(stricmp(ext+extpos, "_f.cg") == 0)
        p = m_fragmentProfile;

    CGprogram program = cgCreateProgram(m_owner->m_context, CG_SOURCE, (const char *)file->basememory(), p, 0, 0);
    cgCompileProgram(program);
    cgGLLoadProgram(program);

    CgShader* s = new CgShader(program);
    CGparameter param = cgGetFirstParameter(program, CG_PROGRAM);
    while(param)
    {
        const char *name = cgGetParameterName(param);
        CgShaderParam* wrapped = 0;
        if(isSystemParameter(name))
        {
            wrapped = getSystemParameter(name);
            cgConnectParameter(wrapped->m_shaderParam, param);
        }
        else
        {
            wrapped = new CgShaderParam(param);
        }
        s->addParam(wrapped);
        param = cgGetNextParameter(param);
    }

    return s;
}

void ShaderPipeline::unload(_Shader* s)
{
    delete s;
}

bool ShaderPipeline::isSystemParameter(const char *name)
{
    return name && name[0] == '_' && name[1] == '_';
}

CgShaderParam* ShaderPipeline::getSystemParameter(const char* name)
{
    return m_systemParams[name].get();
}

const char *ShaderPipeline::getTypeName(ShaderParam::Type t)
{
    return cgGetTypeString(CGtype(t));
}

ShaderParam::Type ShaderPipeline::getTypeByName(const char *name)
{
    CGtype t = cgGetType(name);
    return t;
}

refptr<CgShaderParam> ShaderPipeline::createSystemParameter(const istring& name, ShaderParam::Type type)
{
    CGparameter p = cgCreateParameter(m_owner->m_context, CGtype(type));
    cgSetParameterVariability(p, CG_UNIFORM);
    be_assert(cgGetParameterType(p) == type, "type conflict");
    refptr<CgShaderParam> param(new CgShaderParam(p));
    bool result = m_systemParams.insert(std::make_pair(name, param)).second;
    be_assert(result, "system parameter %s already exists" | name.c_str());
    return param;
}

}}}
