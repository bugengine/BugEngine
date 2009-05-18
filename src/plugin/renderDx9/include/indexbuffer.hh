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

#ifndef BE_DX9_INDEXBUFFER_HH_
#define BE_DX9_INDEXBUFFER_HH_
/*****************************************************************************/
#include    <graphics/renderer/gpubuffer.hh>
#include    <renderer.hh>

#include    <d3d9.h>

namespace BugEngine { namespace Graphics { namespace DirectX9
{

class IndexBuffer : public GpuBuffer
{
    friend class Renderer;
private:
    LPDIRECT3DINDEXBUFFER9 m_buffer;
public:
    IndexBuffer(const Renderer* renderer, size_t count, IndexUsage usage, IndexBufferFlags flags);
    ~IndexBuffer();
protected:
    virtual void* map(GpuMapFlags flags, size_t byteCount, size_t byteOffset) override;
    virtual void  unmap() override;
};

}}}

/*****************************************************************************/
#endif
