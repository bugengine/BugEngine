/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AUTOREGISTRATION_HH_
#define BE_RTTI_AUTOREGISTRATION_HH_
/*****************************************************************************/

namespace BugEngine { namespace RTTI { namespace _
{

template<typename T>
class AutoRegister
{
public:
    AutoRegister()  { T::static_metaclass(); }
    ~AutoRegister() {}
};

}}}

/*****************************************************************************/
#endif
