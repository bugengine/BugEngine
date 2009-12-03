/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_VERTEXBUFFER_HH_
#define BE_GRAPHICS_VERTEXBUFFER_HH_
/*****************************************************************************/
#include    <graphics/renderer/gpubuffer.hh>
#include    <graphics/renderer/vertexdesc.hh>

namespace BugEngine { namespace Graphics
{

class RenderBackend;

template< typename T >
class VertexBuffer : public minitl::refcountable
{
private:
    weak<const RenderBackend> m_owner;
    ref<GpuBuffer>            m_buffer;
public:
    VertexBuffer(weak<const RenderBackend> owner, u32 vertexCount, VertexUsage usage);
    ~VertexBuffer();

    T*  map(u32 count = 0, u32 offset = 0);
    void unmap();

    weak<const GpuBuffer> buffer() const;
};

}}

#include    <graphics/renderer/vertexbuffer.inl>

/*****************************************************************************/
#endif
