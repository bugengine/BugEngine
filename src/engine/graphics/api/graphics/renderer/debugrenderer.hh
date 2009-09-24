/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_DEBUGRENDERER_HH_
#define BE_GRAPHICS_DEBUGRENDERER_HH_
/*****************************************************************************/
#include    <graphics/renderer/vertexbuffer.hh>
#include    <graphics/renderer/indexbuffer.hh>
#include    <graphics/material/shader.hh>
#include    <graphics/material/texture.hh>
#include    <core/debugrenderer/debugrenderer.hh>

namespace BugEngine { namespace Graphics
{

class RenderBackend;
class RenderTarget;

class be_api(GRAPHICS) DebugRenderer : public BugEngine::DebugRenderer
{
    friend class RenderTarget;
protected:
    typedef VertexDescription<MAKEVERTEXLIST3(float2, gPosition, float3, gUV0, color32, gDiffuse)> VertexFormat2D;
    typedef u32 IndexFormat;

    void begin();
    void end();

    template< typename VertexFormat >
    struct PrimitiveBuffer
    {
        refptr< VertexBuffer<VertexFormat> >    m_vertexBuffer;
        VertexFormat*                           m_nextVertex;
        refptr< IndexBuffer<IndexFormat> >      m_indexBuffer;
        IndexFormat*                            m_nextIndex;
        IndexFormat                             m_currentIndex;
        u32                                     m_indexCount;
    };
public:
    DebugRenderer(RenderBackend* renderer);
    ~DebugRenderer();

    uint2 getScreenSize() const override;
    uint2 getCharacterSize() const override;

    void drawLine(int2 p1, int2 p2, color32 color) const override;
    void drawQuad(int2 min, int2 max, color32 color) const override;
    void drawRect(int2 min, int2 max, color32 color) const override;
    void drawText(const char *text, int2 start, color32 color) const override;
private:
    RenderBackend*                  m_renderer;

    mutable PrimitiveBuffer<VertexFormat2D> m_lines;
    mutable PrimitiveBuffer<VertexFormat2D> m_quads;

    Shader                          m_vertexShader;
    Shader                          m_pixelShader;
    Texture                         m_font;
    uint3                           m_characterSize;
};

}}

/*****************************************************************************/
#endif
