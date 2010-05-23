/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/documentation/documentation.hh>
#include    <rtti/engine/struct.hh>

namespace BugEngine { namespace Documentation
{

Doc::Doc(const char * doc)
:   doc(doc)
{
}

Doc::~Doc()
{
}

}

be_struct(Documentation::Doc, void, "Documentation.Doc")
    be_field(doc)
be_end

}
