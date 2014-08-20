/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_HELP_HELP_HH_
#define BE_HELP_HELP_HH_
/**************************************************************************************************/
#include    <stdafx.h>
#include    <bugengine/application.hh>
#include    <plugin/plugin.hh>


namespace BugEngine
{

class Help : public Application
{
public:
    Help(const Plugin::Context& context);
    ~Help();
};

}

/**************************************************************************************************/
#endif
