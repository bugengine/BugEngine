/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <indexbuffer.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

IndexBuffer::IndexBuffer(const Renderer* /*renderer*/, u32 /*vertexCount*/, IndexUsage /*usage*/, IndexBufferFlags /*flags*/)
{
}

IndexBuffer::~IndexBuffer()
{
}

void* IndexBuffer::map(GpuMapFlags /*flags*/, u32 /*byteCount*/, u32 /*byteOffset*/)
{
    return 0;
}

void IndexBuffer::unmap()
{
}


}}}
