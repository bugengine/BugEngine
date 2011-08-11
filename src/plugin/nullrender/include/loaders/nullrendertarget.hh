/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RENDERNULL_LOADERS_NULLRENDERTARGET_HH_
#define BE_RENDERNULL_LOADERS_NULLRENDERTARGET_HH_
/*****************************************************************************/
#include    <3d/renderer/irendertarget.hh>
#include    <3d/renderer/irenderer.hh>


namespace BugEngine { namespace Graphics { namespace Null
{

class NullRenderer;

class NullRenderTarget : public IRenderTarget
{
public:
    NullRenderTarget(weak<const RenderTarget> resource, weak<NullRenderer> renderer);
    ~NullRenderTarget();

private:
    void load(weak<const Resource> resource) override;
    void unload() override;

    void begin(ClearMode clear) const override;
    void end(PresentMode present) const override;

    void present() const;
};

}}}

/*****************************************************************************/
#endif
