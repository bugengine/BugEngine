/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <vertexbuffer.hh>

namespace BugEngine { namespace Graphics { namespace DirectX9
{

VertexBuffer::VertexBuffer(const Renderer* renderer, u32 vertexCount, VertexUsage usage, VertexBufferFlags flags)
{
    DWORD fvf = 0;
    size_t i;
    for(i = 0; i < flags.elementCount; ++i)
    {
        m_vertexElements[i].Stream = 0;
        m_vertexElements[i].Offset = be_checked_numcast<WORD>(flags.info[i].offset);
        switch(flags.info[i].type)
        {
        case VertexElementInfo::float1:
            m_vertexElements[i].Type = D3DDECLTYPE_FLOAT1;
            break;
        case VertexElementInfo::float2:
            m_vertexElements[i].Type = D3DDECLTYPE_FLOAT2;
            break;
        case VertexElementInfo::float3:
            m_vertexElements[i].Type = D3DDECLTYPE_FLOAT3;
            break;
        case VertexElementInfo::float4:
            m_vertexElements[i].Type = D3DDECLTYPE_FLOAT4;
            break;
        case VertexElementInfo::color32:
            m_vertexElements[i].Type = D3DDECLTYPE_UBYTE4N;
            break;
        case VertexElementInfo::byte4:
            m_vertexElements[i].Type = D3DDECLTYPE_UBYTE4N;
            break;
        case VertexElementInfo::short2:
            m_vertexElements[i].Type = D3DDECLTYPE_SHORT2N;
            break;
        case VertexElementInfo::short4:
            m_vertexElements[i].Type = D3DDECLTYPE_SHORT4N;
            break;
        case VertexElementInfo::ushort2:
            m_vertexElements[i].Type = D3DDECLTYPE_USHORT2N;
            break;
        case VertexElementInfo::ushort4:
            m_vertexElements[i].Type = D3DDECLTYPE_USHORT4N;
            break;
        default:
            be_notreached();
        };
        m_vertexElements[i].Method = D3DDECLMETHOD_DEFAULT;
        switch(flags.info[i].semantic)
        {
        case gPosition:
            m_vertexElements[i].Usage = D3DDECLUSAGE_POSITION;
            m_vertexElements[i].UsageIndex = 0;
            fvf |= D3DFVF_XYZ;
            break;
        case gNormal:
            m_vertexElements[i].Usage = D3DDECLUSAGE_NORMAL;
            m_vertexElements[i].UsageIndex = 0;
            fvf |= D3DFVF_NORMAL;
            break;
        case gDiffuse:
            m_vertexElements[i].Usage = D3DDECLUSAGE_COLOR;
            m_vertexElements[i].UsageIndex = 0;
            fvf |= D3DFVF_DIFFUSE;
            break;
        case gSpecular:
            m_vertexElements[i].Usage = D3DDECLUSAGE_COLOR;
            m_vertexElements[i].UsageIndex = 1;
            fvf |= D3DFVF_SPECULAR;
            break;
        case gUV0:
            m_vertexElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
            m_vertexElements[i].UsageIndex = 0;
            fvf |= D3DFVF_TEX0;
            break;
        case gUV1:
            m_vertexElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
            m_vertexElements[i].UsageIndex = 1;
            break;
        case gUV2:
            m_vertexElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
            m_vertexElements[i].UsageIndex = 2;
            break;
        case gUV3:
            m_vertexElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
            m_vertexElements[i].UsageIndex = 3;
            break;
        case gUV4:
            m_vertexElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
            m_vertexElements[i].UsageIndex = 4;
            break;
        case gUV5:
            m_vertexElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
            m_vertexElements[i].UsageIndex = 5;
            break;
        case gUV6:
            m_vertexElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
            m_vertexElements[i].UsageIndex = 6;
            break;
        case gUV7:
            m_vertexElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
            m_vertexElements[i].UsageIndex = 7;
            break;
        default:
            be_notreached();
        }
    }
    D3DVERTEXELEMENT9 end = D3DDECL_END();
    m_vertexElements[i] = end;
    fvf = 0;
    renderer->m_device->CreateVertexBuffer(vertexCount*flags.vertexSize, (usage == VuStatic?0:D3DUSAGE_DYNAMIC) | D3DUSAGE_WRITEONLY, fvf, D3DPOOL_DEFAULT, &m_buffer, 0);
    D3D_CHECKRESULT(renderer->m_device->CreateVertexDeclaration(m_vertexElements, &m_vertexDecl));
    m_vertexStride = flags.vertexSize;
}

VertexBuffer::~VertexBuffer()
{
    m_buffer->Release();
}

void* VertexBuffer::map(GpuMapFlags /*flags*/, u32 byteCount, u32 byteOffset)
{
    void* data;
    D3D_CHECKRESULT(m_buffer->Lock(byteOffset, byteCount, &data, D3DLOCK_DISCARD));
    return data;
}

void VertexBuffer::unmap()
{
    D3D_CHECKRESULT(m_buffer->Unlock());
}


}}}
