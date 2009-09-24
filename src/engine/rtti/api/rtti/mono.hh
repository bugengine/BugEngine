/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_MONO_HH_
#define BE_RTTI_MONO_HH_
/*****************************************************************************/

namespace BugEngine
{

class be_api(RTTI) Mono
{
private:
    void*   m_domain;
public:
    Mono();
    ~Mono();
};

}

/*****************************************************************************/
#endif
