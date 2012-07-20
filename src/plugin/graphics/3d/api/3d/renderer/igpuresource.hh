/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_3D_RENDERER_IGPURESOURCE_HH_
#define BE_3D_RENDERER_IGPURESOURCE_HH_
/*****************************************************************************/
#include    <minitl/refcountable.hh>
#include    <minitl/intrusive_list.hh>

namespace BugEngine
{

class Resource;
template< typename R > class GPUResourceLoader;
class IRenderer;

class be_api(_3D) IGPUResource :   public minitl::refcountable
                               ,   public minitl::intrusive_list<IGPUResource>::item
{
    template< typename T >
    friend class GPUResourceLoader;
    BE_NOCOPY(IGPUResource);
protected:
    const weak<const IRenderer>     m_renderer;
private:
    mutable weak<const Resource>    m_resource;
    mutable i32                     m_index;
public:
    IGPUResource(weak<const Resource> resource, weak<const IRenderer> renderer);
    virtual ~IGPUResource();

    virtual void load(weak<const Resource> resource) = 0;
    virtual void unload() = 0;
};

}

/*****************************************************************************/
#endif
