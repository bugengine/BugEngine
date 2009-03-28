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

#ifndef BE_GRAPHICS_MATERIAL_SHADERPIPELINE_HH_
#define BE_GRAPHICS_MATERIAL_SHADERPIPELINE_HH_
/*****************************************************************************/
#include    <system/resource/pipeline.hh>
#include    <graphics/material/shaderparam.hh>

namespace BugEngine { namespace Graphics
{

class _Shader;

class ShaderPipeline : public System::Pipeline
{
public:
    virtual _Shader*            load(const ifilename& file) = 0;
    virtual void                unload(_Shader* s) = 0;

    virtual ShaderParam::Type   getTypeByName(const char *type) = 0;
    virtual const char *        getTypeName(ShaderParam::Type type) = 0;
};

}}

/*****************************************************************************/
#endif
