/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include   <rtti/stdafx.h>
#include   <rtti/typeinfo.hh>

namespace BugEngine
{

size_t TypeInfo::size() const
{
    switch(type)
    {
    case Class:
        return metaclass->size;
    case Reference:
        return sizeof(void*);
    case RawPtr:
        return sizeof(raw<char>);
    case RefPtr:
        return sizeof(ref<minitl::refcountable>);
    case WeakPtr:
        return sizeof(weak<minitl::refcountable>);
    default:
        be_notreached();
        return 0;
    }
}

}
