/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_PROPERTYINFO_SCRIPT_HH_
#define BE_RTTI_ENGINE_PROPERTYINFO_SCRIPT_HH_
/*****************************************************************************/
#include   <rtti/typeinfo.script.hh>

namespace BugEngine
{
class Value;
}

namespace BugEngine { namespace RTTI
{

class be_api(RTTI) PropertyInfo : public minitl::refcountable
{
    friend class BugEngine::Value;
published:
    const TypeInfo      type;
public:
    PropertyInfo(const TypeInfo& type, Value (*get)(weak<const PropertyInfo> _this, void* from, bool isConst) = 0, void  (*set)(weak<const PropertyInfo> _this, void* from, Value& value, bool isConst) = 0);
    ~PropertyInfo();
private:
    Value (*get)(weak<const PropertyInfo> _this, void* from, bool isConst);
    void  (*set)(weak<const PropertyInfo> _this, void* from, Value& value, bool isConst);
};

}}

/*****************************************************************************/
#endif
