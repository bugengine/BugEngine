/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_CLASSINFO_HH_
#define BE_RTTI_CLASSINFO_HH_
/*****************************************************************************/

namespace BugEngine
{

class ClassInfo : public minitl::refcountable
{
private:
    inamespace m_name;
public:
    ClassInfo(const inamespace& ns);
    virtual ~ClassInfo();

    virtual size_t size() const = 0;
};

}

/*****************************************************************************/
#endif
