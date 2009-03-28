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

#include    <stdafx.h>
#include    <renderer.hh>

#include    <input/inputmap.hh>

#include    <resource.h>
#include    <window.hh>
#include    <texture.hh>
#include    <vertexbuffer.hh>
#include    <indexbuffer.hh>
#include    <texturebuffer.hh>
#include    <cgshader.hh>
#include    <cgshaderparam.hh>

#pragma warning(disable:4311 4312 4355)

namespace BugEngine
{
    extern HINSTANCE hDllInstance;
}


namespace BugEngine { namespace Graphics { namespace DirectX9
{

static void onCgError(CGcontext ctx, CGerror err, void* data)
{
    if(err == cgD3D9Failed)
    {
        AssertMsg(false, cgD3D9TranslateHRESULT(cgD3D9GetLastError()));
    }
    else
    {
        AssertMsg(false, cgD3D9TranslateCGerror(err));
    }
}

Renderer::Renderer()
:   m_directx(Direct3DCreate9(D3D_SDK_VERSION))
,   m_context(cgCreateContext())
,   m_device(0)
,   m_shaderPipeline(this)
,   m_texturePipeline(this)
{
    cgSetErrorHandler(onCgError, 0);
}

Renderer::~Renderer()
{
    int refCnt;

    m_debugRenderer.reset(0);
    cgDestroyContext(m_context);
    cgD3D9SetDevice(0);
    refCnt = m_device->Release();
    AssertMsg(refCnt == 0, "device refcount is not 0");
    refCnt = m_directx->Release();
    AssertMsg(refCnt == 0, "Dx refcount is not 0");
}

LPDIRECT3DSWAPCHAIN9 Renderer::createSwapChain(D3DPRESENT_PARAMETERS* params)
{
    LPDIRECT3DSWAPCHAIN9 result;
    if(m_device)
    {
        D3D_CHECKRESULT(m_device->CreateAdditionalSwapChain(params, &result));
    }
    else
    {
        D3D_CHECKRESULT(m_directx->CreateDevice( D3DADAPTER_DEFAULT,
                                                 D3DDEVTYPE_HAL,
                                                 params->hDeviceWindow,
                                                 D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                                 params,
                                                 &m_device));
        cgD3D9SetDevice(m_device);
        m_systemParams[__Screen] = m_shaderPipeline.createSystemParameter("__screen", m_shaderPipeline.getTypeByName("float2"));
        createDebugRenderer();
        D3D_CHECKRESULT(m_device->GetSwapChain(0, &result));
        D3D_CHECKRESULT(m_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE));
        D3D_CHECKRESULT(m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE));
        D3D_CHECKRESULT(m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE));
        D3D_CHECKRESULT(m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA));
        D3D_CHECKRESULT(m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE));
    }
    return result;
}

RenderTarget* Renderer::createRenderWindow(WindowFlags flags, const Scene* scene)
{
    return new Window(this, flags, scene);
}

GpuBuffer* Renderer::createVertexBuffer(size_t vertexCount, VertexUsage usage, VertexBufferFlags flags) const
{
    return new VertexBuffer(this, vertexCount, usage, flags);
}

GpuBuffer* Renderer::createIndexBuffer(size_t vertexCount, IndexUsage usage, IndexBufferFlags flags) const
{
    return new IndexBuffer(this, vertexCount, usage, flags);
}

GpuBuffer* Renderer::createTextureBuffer(TextureBufferFlags flags) const
{
    return 0;
}

ShaderPipeline* Renderer::getShaderPipeline()
{
    return &m_shaderPipeline;
}

TexturePipeline* Renderer::getTexturePipeline()
{
    return &m_texturePipeline;
}

DebugRenderer* Renderer::debugRenderer()
{
    return m_debugRenderer.get();
}

void Renderer::createDebugRenderer()
{
    Assert(!m_debugRenderer);
    m_debugRenderer.reset(new DebugRenderer(this));
}

void Renderer::drawBatch(const Batch& b)
{
    const VertexBuffer* _vb = static_cast<const VertexBuffer*>(b.vertices);
    const IndexBuffer* _ib = static_cast<const IndexBuffer*>(b.indices);

    D3D_CHECKRESULT(m_device->SetVertexDeclaration(_vb->m_vertexDecl));
    D3D_CHECKRESULT(m_device->SetStreamSource(0, _vb->m_buffer, 0, _vb->m_vertexStride));
    D3D_CHECKRESULT(m_device->SetIndices(_ib->m_buffer));

    for(size_t i = 0; i < b.nbParams; ++i)
        b.params[i].first->setValue(b.params[i].second);

    if(b.vertexShader) static_cast<const CgShader*>(b.vertexShader)->set();
    if(b.pixelShader) static_cast<const CgShader*>(b.pixelShader)->set();

    D3DPRIMITIVETYPE type;
    int primitiveCount = 0;
    switch(b.ptype)
    {
    case Batch::RptLineList:
        type = D3DPT_LINELIST;
        primitiveCount = b.nbVertices/2;
        break;
    case Batch::RptTriangleList:
        type = D3DPT_TRIANGLELIST;
        primitiveCount = b.nbVertices/3;
        break;
    case Batch::RptTriangleStrip:
    case Batch::RptTriangleFan:
    default: AssertNotReached(); type = D3DPT_TRIANGLELIST; primitiveCount = b.nbVertices;
    }
    D3D_CHECKRESULT(m_device->DrawIndexedPrimitive(type, 0, 0, b.nbVertices, 0, primitiveCount));
}

void Renderer::flush() const
{
}

}}}
