/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <renderer.hh>

#include    <input/inputmap.hh>

#include    <window.hh>


namespace BugEngine { namespace Graphics { namespace OpenGL
{

Renderer::Renderer(weak<const FileSystem> filesystem)
:   m_glContext(0)
,   m_filesystem(filesystem)
{
}

ref<IRenderTarget> Renderer::createRenderWindow(WindowFlags flags)
{
    return ref<Window>::create<Arena::General>(this, flags);
}

ref<IRenderTarget> Renderer::createRenderBuffer(TextureFlags /*flags*/)
{
    return ref<Window>();
}

ref<IRenderTarget> Renderer::createMultipleRenderBuffer(TextureFlags /*flags*/, size_t /*count*/)
{
    return ref<Window>();
}

void Renderer::drawBatch(const Batch& b)
{
    //weak<const VertexBuffer> _vb = be_checked_cast<const VertexBuffer>(b.vertices);
    //weak<const IndexBuffer> _ib = be_checked_cast<const IndexBuffer>(b.indices);
    //be_forceuse(_vb);
    //be_forceuse(_ib);

    switch(b.ptype)
    {
    case Batch::RptLineList:
        break;
    case Batch::RptTriangleList:
        break;
    case Batch::RptTriangleStrip:
    case Batch::RptTriangleFan:
    default:
        be_notreached();
    }
}

void Renderer::flush()
{
    Windowing::Renderer::flush();
    static Timer t;
    static int frames = 0;
    if(++frames % 100 == 0)
        OutputDebugString((minitl::format<>("%d\n") | (100/t.tick())).c_str());
}

}}}
