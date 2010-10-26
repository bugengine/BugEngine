/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_NAMESPACE_HH_
#define BE_RTTI_NAMESPACE_HH_
/*****************************************************************************/
#include    <rtti/engine/classinfo.script.hh>

namespace BugEngine
{
class Value;
}

namespace BugEngine { namespace RTTI
{

class Namespace : public minitl::refcountable
{
    friend class ::BugEngine::Value;
private:
    raw<const ClassInfo>    m_classInfo;
public:
    Namespace();
    ~Namespace();

    void add(const istring name, Value value);
};

}}

/*****************************************************************************/
#endif
