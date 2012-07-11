/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_RESOURCE_PIPELINE_HH_
#define BE_SYSTEM_RESOURCE_PIPELINE_HH_
/*****************************************************************************/
#include    <minitl/ptr/refcountable.hh>
#include    <system/resource/resourceloader.hh>

namespace BugEngine
{

template< typename Owner, typename R >
class Pipeline : public ResourceLoader<Owner, R>
{
protected:
    typedef typename ResourceLoader<Owner, R>::LoadMethod   LoadMethod;
    typedef typename ResourceLoader<Owner, R>::UnloadMethod UnloadMethod;
public:
    Pipeline(LoadMethod load, UnloadMethod unload);
    ~Pipeline();

    
};

}

/*****************************************************************************/
#endif
