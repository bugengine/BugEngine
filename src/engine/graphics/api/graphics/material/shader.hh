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

#ifndef BE_GRAPHICS_MATERIAL_SHADER_HH_
#define BE_GRAPHICS_MATERIAL_SHADER_HH_
/*****************************************************************************/
#include    <system/resource/resource.hh>
#include    <graphics/material/shaderpipeline.hh>

namespace BugEngine { namespace Graphics
{

class ShaderParam;

class GRAPHICSEXPORT _Shader
{
public:
    typedef ShaderPipeline  Pipeline;
public:
    _Shader();
    virtual ~_Shader();

    virtual ShaderParam* getParam(const istring& name) const = 0;
};

typedef Resource<_Shader>   Shader;

}}

/*****************************************************************************/
#endif
