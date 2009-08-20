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

#ifndef BE_GRAPHICS_MATERIAL_SHADERPARAM_HH_
#define BE_GRAPHICS_MATERIAL_SHADERPARAM_HH_
/*****************************************************************************/
#include    <minitl/ptr/refcountable.hh>
#include    <graphics/material/texture.hh>

namespace BugEngine { namespace Graphics
{

class GRAPHICSEXPORT ShaderParam : public minitl::refcountable<void>
{
public:
    typedef int Type;
public:
    ShaderParam();
    ~ShaderParam();

    virtual void setValue(float4 value) = 0;
    virtual void setValue(_Texture* value) = 0;
    virtual Type type() const = 0;

    virtual const char *name() const = 0;
};

}}

/*****************************************************************************/
#endif
