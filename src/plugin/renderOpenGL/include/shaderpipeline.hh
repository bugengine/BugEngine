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

#ifndef BE_OPENGL_SHADERPIPELINE_HH_
#define BE_OPENGL_SHADERPIPELINE_HH_
/*****************************************************************************/
#include    <graphics/material/shaderpipeline.hh>
#include    <graphics/material/shaderparam.hh>
#include    <Cg/cg.h>
#include    <Cg/cgGL.h>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

class Renderer;
class CgShaderParam;

class ShaderPipeline : public Graphics::ShaderPipeline
{
private:
    Renderer*                                   m_owner;
    CGprofile                                   m_vertexProfile;
    CGprofile                                   m_fragmentProfile;
    std::map< istring, refptr<CgShaderParam> >  m_systemParams;
public:
    ShaderPipeline(Renderer* owner);
    ~ShaderPipeline();

    _Shader*                load(const ifilename& file) override;
    void                    unload(_Shader* s) override;

    ShaderParam::Type       getTypeByName(const char *type) override;
    const char *            getTypeName(ShaderParam::Type type) override;

    refptr<CgShaderParam>   createSystemParameter(const istring& name, ShaderParam::Type type);
private:
    bool                    isSystemParameter(const char *name);
    CgShaderParam*          getSystemParameter(const char *name);
};

}}}

/*****************************************************************************/
#endif
