/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_INDEXBUFFER_HH_
#define BE_GRAPHICS_INDEXBUFFER_HH_
/*****************************************************************************/
#include    <graphics/renderer/gpubuffer.hh>
#include    <graphics/renderer/vertexdesc.hh>

namespace BugEngine { namespace Graphics
{

class RenderBackend;

template< typename T >
class IndexBuffer : public minitl::refcountable<void>
{
private:
    const RenderBackend*    m_owner;
    refptr<GpuBuffer>       m_buffer;
public:
    IndexBuffer(const RenderBackend* owner, u32 vertexCount, IndexUsage usage);
    ~IndexBuffer();

    T*  map(u32 count = 0, u32 offset = 0);
    void unmap();

    const GpuBuffer* buffer() const;
};

}}

#include    <graphics/renderer/indexbuffer.inl>

/*****************************************************************************/
#endif
