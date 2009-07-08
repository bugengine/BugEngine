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

#ifndef BE_GRAPHICS_PRIMITIVE_TEXT_HH_
#define BE_GRAPHICS_PRIMITIVE_TEXT_HH_
/*****************************************************************************/
#include    <graphics/primitive/primitive.hh>
#include    <graphics/primitive/font.hh>

namespace BugEngine { namespace Graphics
{

class GRAPHICSEXPORT Text : public Primitive
{
private:
    refptr<const Font>          m_font;
    refptr< IndexBuffer<u16> >  m_indexBuffer;
    int                         m_totalVertices;
public:
    Text(const RenderBackend* renderer, const Font* font, const char* m);
    ~Text();

    virtual const GpuBuffer* indices() const override;
    virtual const GpuBuffer* vertices() const override;
    virtual unsigned         nbVertices() const override;

    be_metaclass(GRAPHICSEXPORT,Text,Primitive)
    be_properties
    be_end
};

}}

/*****************************************************************************/
#endif
