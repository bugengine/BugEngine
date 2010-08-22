/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DX9_RENDERTARGETS_MULTIPLE_HH_
#define BE_DX9_RENDERTARGETS_MULTIPLE_HH_
/*****************************************************************************/
#include    <graphics/renderer/irendertarget.hh>
#include    <graphics/renderer/irenderer.hh>
#include    <d3d9.h>

namespace BugEngine { namespace Graphics { namespace DirectX9
{

class Renderer;

class MultipleRenderTarget : public Graphics::IRenderTarget
{
private:
    void setCurrent();
public:
    MultipleRenderTarget(weak<Renderer> renderer, TextureFlags flags);
    ~MultipleRenderTarget();

    void begin(ClearMode clear) override;
    void end(PresentMode present) override;

    void close() override;
    bool closed() const override;
};

}}}

/*****************************************************************************/
#endif
