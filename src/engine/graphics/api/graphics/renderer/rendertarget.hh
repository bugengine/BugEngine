/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERTARGET_HH_
#define BE_GRAPHICS_RENDERTARGET_HH_
/*****************************************************************************/
#include    <graphics/renderer/debugrenderer.hh>

namespace BugEngine { namespace Graphics
{

class Scene;

class GRAPHICSEXPORT RenderTarget : public minitl::refcountable<void>
{
private:
    refptr<const Scene> const   m_scene;
public:
    RenderTarget(const Scene* scene);
    virtual ~RenderTarget() { }
    virtual void setCurrent() = 0;

    virtual DebugRenderer*  debugRenderer() = 0;
    virtual uint2           getDimensions() const = 0;
    virtual void            close() = 0;
    virtual bool            closed() const = 0;

    const Scene* scene() const;
private:
    RenderTarget& operator=(const RenderTarget& other);
    RenderTarget(const RenderTarget& other);
};

}}

/*****************************************************************************/
#endif
