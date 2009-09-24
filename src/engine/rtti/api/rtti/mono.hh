/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_MONO_HH_
#define BE_RTTI_MONO_HH_
/*****************************************************************************/
#include    <core/utils/singleton.hh>

namespace BugEngine
{

class be_api(RTTI) Mono : public Singleton<Mono>
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
