/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <shaderpipeline.hh>
#include    <renderer.hh>

#include    <core/memory/streams.hh>
#include    <system/filesystem.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

ShaderPipeline::ShaderPipeline(Renderer* owner)
    :   m_owner(owner)
{
}

ShaderPipeline::~ShaderPipeline()
{
}

_Shader* ShaderPipeline::load(const ifilename& /*filename*/)
{
    return 0;
}

void ShaderPipeline::unload(_Shader* /*s*/)
{
}

const char *ShaderPipeline::getTypeName(ShaderParam::Type /*t*/)
{
    return 0;
}

ShaderParam::Type ShaderPipeline::getTypeByName(const char * /*name*/)
{
    return 0;
}

}}}
