/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_RENDERER_HH_
#define BE_OPENGL_RENDERER_HH_
/*****************************************************************************/
#include    <cgshaderparam.hh>
#include    <Cg/cg.h>
#include    <Cg/cgGL.h>

#include    <shaderpipeline.hh>
#include    <texturepipeline.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

class Window;
class VertexBuffer;
class IndexBuffer;
class TextureBuffer;

class Renderer : public Windowing::Renderer
{
    friend class Window;
    friend class VertexBuffer;
    friend class IndexBuffer;
    friend class TextureBuffer;
    friend class ShaderPipeline;
    friend class TexturePipeline;
private:
    enum
    {
        __Screen,
        SystemParamCount
    };
private:
#   ifdef BE_PLATFORM_WIN32
    HGLRC                       m_glContext;
#   else
    GLXContext                  m_glContext;
#   endif
    CGcontext                   m_context;
    ShaderPipeline              m_shaderPipeline;
    TexturePipeline             m_texturePipeline;
    refptr<CgShaderParam>       m_systemParams[SystemParamCount];
public:
    Renderer();
    ~Renderer();

    ShaderPipeline*         getShaderPipeline() override;
    TexturePipeline*        getTexturePipeline() override;

    RenderTarget*           createRenderWindow(WindowFlags flags, const Scene* scene) override;
    GpuBuffer*              createVertexBuffer(u32 vertexCount, VertexUsage usage, VertexBufferFlags flags) const override;
    GpuBuffer*              createIndexBuffer(u32 vertexCount, IndexUsage usage, IndexBufferFlags flags) const override;
    GpuBuffer*              createTextureBuffer(TextureBufferFlags flags) const override;

    void                    drawBatch(const Batch& b) override;
    void                    flush() const override;
private:
    void                    attachWindow(Window* w);
};

}}}

/*****************************************************************************/
#endif
