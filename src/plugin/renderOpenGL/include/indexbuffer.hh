/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_INDEXBUFFER_HH_
#define BE_OPENGL_INDEXBUFFER_HH_
/*****************************************************************************/
#include    <graphics/renderer/gpubuffer.hh>
#include    <renderer.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

class IndexBuffer : public GpuBuffer
{
    friend class Renderer;
private:
    //LPDIRECT3DINDEXBUFFER9 m_buffer;
public:
    IndexBuffer(const Renderer* renderer, u32 count, IndexUsage usage, IndexBufferFlags flags);
    ~IndexBuffer();
protected:
    virtual void* map(GpuMapFlags flags, u32 byteCount, u32 byteOffset) override;
    virtual void  unmap() override;
};

}}}

/*****************************************************************************/
#endif
