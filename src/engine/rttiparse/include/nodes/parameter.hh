/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_NODES_PARAMETER_HH_
#define BE_RTTIPARSE_NODES_PARAMETER_HH_
/**************************************************************************************************/
#include    <rttiparse/stdafx.h>
#include    <rttiparse/valueparse.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

struct Parameter
{
    ParseLocation   location;
    istring         name;
    ref<Node>       value;
};

}}}

/**************************************************************************************************/
#endif
