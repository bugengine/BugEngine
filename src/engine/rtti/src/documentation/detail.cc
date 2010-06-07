/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/documentation/detail.hh>
#include    <rtti/engine/struct.hh>

namespace BugEngine { namespace Documentation
{

Detail::Detail(const char * doc)
:   doc(doc)
{
}

Detail::~Detail()
{
}

}

be_struct(Documentation::Detail, void, "Documentation.Detail")
    be_field(doc)
be_end

}
