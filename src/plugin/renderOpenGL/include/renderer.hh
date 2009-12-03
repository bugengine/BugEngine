/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_RENDERER_HH_
#define BE_OPENGL_RENDERER_HH_
/*****************************************************************************/
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
    ShaderPipeline              m_shaderPipeline;
    TexturePipeline             m_texturePipeline;
public:
    Renderer();
    ~Renderer();

    weak<ShaderPipeline>    getShaderPipeline() override;
    weak<TexturePipeline>   getTexturePipeline() override;

    ref<RenderTarget>       createRenderWindow(WindowFlags flags, weak<const Scene> scene) override;
    ref<GpuBuffer>          createVertexBuffer(u32 vertexCount, VertexUsage usage, VertexBufferFlags flags) const override;
    ref<GpuBuffer>          createIndexBuffer(u32 vertexCount, IndexUsage usage, IndexBufferFlags flags) const override;
    ref<GpuBuffer>          createTextureBuffer(TextureBufferFlags flags) const override;

    void                    drawBatch(const Batch& b) override;
    void                    flush() const override;
private:
    void                    attachWindow(Window* w);
};

}}}

/*****************************************************************************/
#endif
