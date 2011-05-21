/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_IGPURESOURCE_HH_
#define BE_GRAPHICS_RENDERER_IGPURESOURCE_HH_
/*****************************************************************************/

namespace BugEngine { class Resource; }

namespace BugEngine { namespace Graphics
{

class IRenderer;

class be_api(GRAPHICS) IGPUResource : public minitl::inode
                                    , public minitl::intrusive_list<IGPUResource>::item
{
protected:
    const weak<const Resource>  m_resource;
public:
    IGPUResource(weak<const Resource> resource);
    virtual ~IGPUResource();

    virtual void load(weak<IRenderer> renderer) = 0;
    virtual void unload(weak<IRenderer> renderer) = 0;
};

}}

/*****************************************************************************/
#endif
