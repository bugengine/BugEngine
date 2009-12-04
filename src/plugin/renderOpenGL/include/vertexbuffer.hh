/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_VERTEXBUFFER_HH_
#define BE_OPENGL_VERTEXBUFFER_HH_
/*****************************************************************************/
#include    <graphics/renderer/gpubuffer.hh>
#include    <graphics/renderer/vertexdesc.hh>
#include    <renderer.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

class VertexBuffer : public GpuBuffer
{
    friend class Renderer;
private:
    //LPDIRECT3DVERTEXBUFFER9         m_buffer;
    //D3DVERTEXELEMENT9               m_vertexElements[MAXD3DDECLLENGTH];
    //LPDIRECT3DVERTEXDECLARATION9    m_vertexDecl;
    u32                             m_vertexStride;
public:
    VertexBuffer(weak<const Renderer> renderer, u32 count, VertexUsage usage, VertexBufferFlags flags);
    ~VertexBuffer();
protected:
    virtual void* map(GpuMapFlags flags, u32 byteCount, u32 byteOffset) override;
    virtual void  unmap() override;
};

}}}

/*****************************************************************************/
#endif
