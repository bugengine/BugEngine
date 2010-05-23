/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/documentation/brief.hh>
#include    <rtti/engine/struct.hh>

namespace BugEngine { namespace Documentation
{

Brief::Brief(const char * brief)
:   doc(brief)
{
}

Brief::~Brief()
{
}

}

be_struct(Documentation::Brief, void, "Documentation.Brief")
    be_field(doc)
be_end

}
