/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DX9_RENDERTARGETS_OFFSCREEN_HH_
#define BE_DX9_RENDERTARGETS_OFFSCREEN_HH_
/*****************************************************************************/
#include    <graphics/renderer/irendertarget.hh>
#include    <graphics/renderer/irenderer.hh>
#include    <d3d9.h>

namespace BugEngine { namespace Graphics { namespace DirectX9
{

class Renderer;

class OffscreenRenderTarget : public Graphics::IRenderTarget
{
private:
    LPDIRECT3DSURFACE9    m_surface;
private:
    void setCurrent();
public:
    OffscreenRenderTarget(weak<Renderer> renderer, TextureFlags flags);
    ~OffscreenRenderTarget();

    void begin(ClearMode clear) override;
    void end(PresentMode present) override;

    void close() override;
    bool closed() const override;
};

}}}

/*****************************************************************************/
#endif
