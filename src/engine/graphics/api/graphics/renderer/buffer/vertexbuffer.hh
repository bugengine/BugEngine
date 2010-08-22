/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_BUFFER_VERTEXBUFFER_HH_
#define BE_GRAPHICS_RENDERER_BUFFER_VERTEXBUFFER_HH_
/*****************************************************************************/
#include    <graphics/renderer/buffer/gpubuffer.hh>
#include    <graphics/renderer/buffer/vertexdesc.hh>

namespace BugEngine { namespace Graphics
{

template< typename T >
class VertexBuffer : public GpuBuffer
{
public:
    VertexBuffer(u32 vertexCount, VertexUsage usage);
    ~VertexBuffer();

    T*  map(u32 count = 0, u32 offset = 0);
    void unmap();
};

}}

#include    <graphics/renderer/vertexbuffer.inl>

/*****************************************************************************/
#endif
