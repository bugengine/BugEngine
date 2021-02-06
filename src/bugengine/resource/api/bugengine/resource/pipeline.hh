/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RESOURCE_PIPELINE_HH_
#define BE_RESOURCE_PIPELINE_HH_
/**************************************************************************************************/
#include <bugengine/resource/stdafx.h>
#include <bugengine/minitl/ptr/refcountable.hh>
#include <bugengine/resource/resourceloader.hh>

namespace BugEngine {

template < typename Owner, typename R >
class Pipeline : public ResourceLoader< Owner, R >
{
protected:
    typedef typename ResourceLoader< Owner, R >::LoadMethod   LoadMethod;
    typedef typename ResourceLoader< Owner, R >::UnloadMethod UnloadMethod;

public:
    Pipeline(LoadMethod load, UnloadMethod unload);
    ~Pipeline();
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
