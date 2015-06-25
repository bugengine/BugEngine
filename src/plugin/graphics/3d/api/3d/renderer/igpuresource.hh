/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_3D_RENDERER_IGPURESOURCE_HH_
#define BE_3D_RENDERER_IGPURESOURCE_HH_
/**************************************************************************************************/
#include    <3d/stdafx.h>
#include    <minitl/refcountable.hh>
#include    <minitl/intrusive_list.hh>

namespace BugEngine
{

namespace Resource
{
class Description;
}

template< typename R > class GPUResourceLoader;
class IRenderer;

class be_api(_3D) IGPUResource :   public minitl::refcountable
                               ,   public minitl::intrusive_list<IGPUResource>::item
{
    template< typename T >
    friend class GPUResourceLoader;
    BE_NOCOPY(IGPUResource);
protected:
    const weak<const IRenderer>         m_renderer;
private:
    weak<const Resource::Description>   m_resource;
    i32                                 m_index;
public:
    IGPUResource(weak<const Resource::Description> description, weak<const IRenderer> renderer);
    virtual ~IGPUResource();

    virtual void load(weak<const Resource::Description> description) = 0;
    virtual void unload() = 0;
};

}

/**************************************************************************************************/
#endif
