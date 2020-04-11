/* BugEngine <bugengine.devel@gmail.com> under New BSD License
   see LICENSE for detail */

#ifndef BE_TEST_COMPUTE_COPY_COPY_HH_
#define BE_TEST_COMPUTE_COPY_COPY_HH_
/**************************************************************************************************/
#include    <stdafx.h>
#include    <bugengine/application.hh>


namespace BugEngine { namespace Test { namespace Compute { namespace Copy
{

class CopyApplication : public Application
{
public:
    CopyApplication(const Plugin::Context& context);
    ~CopyApplication();
};

}}}}

/**************************************************************************************************/
#endif
