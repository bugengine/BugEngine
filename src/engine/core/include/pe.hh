/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_DEBUG_PE_HH_
#define BE_CORE_DEBUG_PE_HH_
/*****************************************************************************/

namespace BugEngine { namespace Debug
{

class PE
{
public:
    PE(const char* filename, FILE* f);
    ~PE();

};

}}

/*****************************************************************************/
#endif

