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

#include    <graphics/stdafx.h>
#include    <graphics/renderer/debugrenderer.hh>

namespace BugEngine { namespace Graphics
{

static const size_t g_debugRendererVertexCount = 200000;

DebugRenderer::DebugRenderer(RenderBackend* renderer)
:   m_renderer(renderer)
,   m_vertexShader(renderer->getShaderPipeline(), "data/shaders/debugRenderer.cg")
,   m_pixelShader(renderer->getShaderPipeline(), "data/shaders/debugRenderer_f.cg")
//,   m_font(renderer->getTexturePipeline(), "data/fonts/profont.tga")
//,   m_characterSize(8,10,5)
,   m_font(renderer->getTexturePipeline(), "data/fonts/frasto.tga")
,   m_characterSize(8,12,7)
{
    m_pixelShader->getParam("font")->setValue(m_font.get());
    m_vertexShader->getParam("fontdim")->setValue(float4(float(m_characterSize.x()), float(m_characterSize.y()), float(m_characterSize.z()), 16.0f));
    m_lines.m_vertexBuffer = new VertexBuffer<VertexFormat2D>(renderer, g_debugRendererVertexCount, VuStatic);
    m_lines.m_indexBuffer = new IndexBuffer<IndexFormat>(renderer, g_debugRendererVertexCount, IuStatic);
    m_quads.m_vertexBuffer = new VertexBuffer<VertexFormat2D>(renderer, g_debugRendererVertexCount, VuStatic);
    m_quads.m_indexBuffer = new IndexBuffer<IndexFormat>(renderer, g_debugRendererVertexCount, IuStatic);
}

DebugRenderer::~DebugRenderer()
{
}

void DebugRenderer::begin()
{
    m_lines.m_nextVertex = m_lines.m_vertexBuffer->map();
    m_lines.m_nextIndex = m_lines.m_indexBuffer->map();
    m_lines.m_currentIndex = 0;
    m_lines.m_indexCount = 0;

    m_quads.m_nextVertex = m_quads.m_vertexBuffer->map();
    m_quads.m_nextIndex = m_quads.m_indexBuffer->map();
    m_quads.m_currentIndex = 0;
    m_quads.m_indexCount = 0;
}

void DebugRenderer::end()
{
    m_lines.m_vertexBuffer->unmap();
    m_lines.m_nextVertex = 0;
    m_lines.m_indexBuffer->unmap();
    m_lines.m_nextIndex = 0;

    Batch b;
    b.vertices = m_lines.m_vertexBuffer->buffer();
    b.indices  = m_lines.m_indexBuffer->buffer();
    b.nbVertices =  m_lines.m_indexCount;
    b.pixelShader = m_pixelShader.get();
    b.vertexShader = m_vertexShader.get();
    b.ptype = Batch::RptLineList;
    b.nbParams = 0;
    
    m_renderer->drawBatch(b);

    m_quads.m_vertexBuffer->unmap();
    m_quads.m_nextVertex = 0;
    m_quads.m_indexBuffer->unmap();
    m_quads.m_nextIndex = 0;

    b.vertices = m_quads.m_vertexBuffer->buffer();
    b.indices  = m_quads.m_indexBuffer->buffer();
    b.nbVertices =  m_quads.m_indexCount;
    b.pixelShader = m_pixelShader.get();
    b.vertexShader = m_vertexShader.get();
    b.ptype = Batch::RptTriangleList;

    m_renderer->drawBatch(b);
}

uint2 DebugRenderer::getScreenSize() const
{
    return m_renderer->getScreenSize();
}

uint2 DebugRenderer::getCharacterSize() const
{
    return m_characterSize.zy();
}

void DebugRenderer::drawLine(int2 p1, int2 p2, color32 color) const
{
    m_lines.m_nextVertex->position = float2(p1.xy());
    m_lines.m_nextVertex->diffuse = color;
    m_lines.m_nextVertex->uv0.xy() = float2(0.0f, 0.0f);
    m_lines.m_nextVertex->uv0.z() = 0.0;
    m_lines.m_nextVertex++;

    m_lines.m_nextVertex->position = float2(p2.xy());
    m_lines.m_nextVertex->diffuse = color;
    m_lines.m_nextVertex->uv0.xy() = float2(1.0f, 1.0f);
    m_lines.m_nextVertex->uv0.z() = 0.0;
    m_lines.m_nextVertex++;

    *(m_lines.m_nextIndex++) = m_lines.m_currentIndex;
    *(m_lines.m_nextIndex++) = m_lines.m_currentIndex+1;

    m_lines.m_currentIndex += 2;
    m_lines.m_indexCount += 2;
}

void DebugRenderer::drawQuad(int2 min, int2 max, color32 color) const
{
    int2 c1(min);
    int2 c2(max.x(), min.y());
    int2 c3(max);
    int2 c4(min.x(), max.y());

    m_lines.m_nextVertex->position = float2(c1.xy());
    m_lines.m_nextVertex->diffuse = color;
    m_lines.m_nextVertex->uv0.xy() = float2(0.0f, 0.0f);
    m_lines.m_nextVertex->uv0.z() = 0.0;
    m_lines.m_nextVertex++;

    m_lines.m_nextVertex->position = float2(c2.xy());
    m_lines.m_nextVertex->diffuse = color;
    m_lines.m_nextVertex->uv0.xy() = float2(1.0f, 0.0f);
    m_lines.m_nextVertex->uv0.z() = 0.0;
    m_lines.m_nextVertex++;

    m_lines.m_nextVertex->position = float2(c3.xy());
    m_lines.m_nextVertex->diffuse = color;
    m_lines.m_nextVertex->uv0.xy() = float2(1.0f, 1.0f);
    m_lines.m_nextVertex->uv0.z() = 0.0;
    m_lines.m_nextVertex++;

    m_lines.m_nextVertex->position = float2(c4.xy());
    m_lines.m_nextVertex->diffuse = color;
    m_lines.m_nextVertex->uv0.xy() = float2(0.0f, 1.0f);
    m_lines.m_nextVertex->uv0.z() = 0.0;
    m_lines.m_nextVertex++;


    *(m_lines.m_nextIndex++) = m_lines.m_currentIndex;
    *(m_lines.m_nextIndex++) = m_lines.m_currentIndex+1;
    *(m_lines.m_nextIndex++) = m_lines.m_currentIndex+1;
    *(m_lines.m_nextIndex++) = m_lines.m_currentIndex+2;
    *(m_lines.m_nextIndex++) = m_lines.m_currentIndex+2;
    *(m_lines.m_nextIndex++) = m_lines.m_currentIndex+3;
    *(m_lines.m_nextIndex++) = m_lines.m_currentIndex+3;
    *(m_lines.m_nextIndex++) = m_lines.m_currentIndex;

    m_lines.m_currentIndex += 4;
    m_lines.m_indexCount += 8;
}

void DebugRenderer::drawRect(int2 min, int2 max, color32 color) const
{
    int2 c1(min);
    int2 c2(max.x(), min.y());
    int2 c3(max);
    int2 c4(min.x(), max.y());

    m_quads.m_nextVertex->position = float2(c1.xy());
    m_quads.m_nextVertex->diffuse = color;
    m_quads.m_nextVertex->uv0.xy() = float2(0.0f, 0.0f);
    m_quads.m_nextVertex->uv0.z() = 0.0;
    m_quads.m_nextVertex++;

    m_quads.m_nextVertex->position = float2(c2.xy());
    m_quads.m_nextVertex->diffuse = color;
    m_quads.m_nextVertex->uv0.xy() = float2(1.0f, 0.0f);
    m_quads.m_nextVertex->uv0.z() = 0.0;
    m_quads.m_nextVertex++;

    m_quads.m_nextVertex->position = float2(c3.xy());
    m_quads.m_nextVertex->diffuse = color;
    m_quads.m_nextVertex->uv0.xy() = float2(1.0f, 1.0f);
    m_quads.m_nextVertex->uv0.z() = 0.0;
    m_quads.m_nextVertex++;

    m_quads.m_nextVertex->position = float2(c4.xy());
    m_quads.m_nextVertex->diffuse = color;
    m_quads.m_nextVertex->uv0.xy() = float2(0.0f, 1.0f);
    m_quads.m_nextVertex->uv0.z() = 0.0;
    m_quads.m_nextVertex++;


    *(m_quads.m_nextIndex++) = m_quads.m_currentIndex;
    *(m_quads.m_nextIndex++) = m_quads.m_currentIndex+1;
    *(m_quads.m_nextIndex++) = m_quads.m_currentIndex+3;
    *(m_quads.m_nextIndex++) = m_quads.m_currentIndex+1;
    *(m_quads.m_nextIndex++) = m_quads.m_currentIndex+2;
    *(m_quads.m_nextIndex++) = m_quads.m_currentIndex+3;

    m_quads.m_currentIndex += 4;
    m_quads.m_indexCount += 6;
}

void DebugRenderer::drawText(const char *text, int2 start, color32 color) const
{
    float offset = float(m_characterSize.z());
    float2 c1(start);
    float2 c2(start.x()+offset, float(start.y()));
    float2 c3(start.x()+offset, float(start.y()+m_characterSize.y()));
    float2 c4(float(start.x()), float(start.y()+m_characterSize.y()));

    float2 max = m_renderer->getScreenSize();
    float2 min(0.0f, 0.0f);

    if(c3.x() < 0.0f && c3.y() < 0.0f)
        return;
    if(c1.y() > max.y())
        return;

    VertexFormat2D v[4];
    v[0].position.xy() = c1.xy();
    v[0].diffuse = color;
    v[0].uv0.xy() = float2(0.0f, 0.0f);
    v[1].position.xy() = c2.xy();
    v[1].diffuse = color;
    v[1].uv0.xy() = float2(1.0f, 0.0f);
    v[2].position.xy() = c3.xy();
    v[2].diffuse = color;
    v[2].uv0.xy() = float2(1.0f, 1.0f);
    v[3].position.xy() = c4.xy();
    v[3].diffuse = color;
    v[3].uv0.xy() = float2(0.0f, 1.0f);

    for(; *text; text++)
    {
        if(text[0] == '\r')
            continue;
        else if(text[0] == '\t')
        {
            v[0].position.x() += offset*2;
            v[1].position.x() += offset*2;
            v[2].position.x() += offset*2;
            v[3].position.x() += offset*2;
            continue;
        }
        else if(text[0] == '\n')
        {
            c1.y() += m_characterSize.y();
            c2.y() += m_characterSize.y();
            c3.y() += m_characterSize.y();
            c4.y() += m_characterSize.y();
            v[0].position.xy() = c1.xy();
            v[1].position.xy() = c2.xy();
            v[2].position.xy() = c3.xy();
            v[3].position.xy() = c4.xy();
            continue;
        }

        float val = text[0];
        v[0].uv0.z() = val;
        v[1].uv0.z() = val;
        v[2].uv0.z() = val;
        v[3].uv0.z() = val;

        *(m_quads.m_nextVertex + 0) = v[0];
        *(m_quads.m_nextIndex+0) = m_quads.m_currentIndex;
        *(m_quads.m_nextIndex+1) = m_quads.m_currentIndex+1;
        *(m_quads.m_nextVertex + 1) = v[1];
        *(m_quads.m_nextIndex+2) = m_quads.m_currentIndex+3;
        *(m_quads.m_nextIndex+3) = m_quads.m_currentIndex+1;
        *(m_quads.m_nextVertex + 2) = v[2];
        *(m_quads.m_nextIndex+4) = m_quads.m_currentIndex+2;
        *(m_quads.m_nextIndex+5) = m_quads.m_currentIndex+3;
        *(m_quads.m_nextVertex + 3) = v[3];

        v[0].position.x() += offset;
        v[1].position.x() += offset;
        v[2].position.x() += offset;
        v[3].position.x() += offset;

        m_quads.m_nextVertex += 4;
        m_quads.m_nextIndex += 6;

        m_quads.m_currentIndex += 4;
        m_quads.m_indexCount += 6;
    }
}

}}
