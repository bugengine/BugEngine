/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/primitive/primitive.hh>

namespace BugEngine { namespace Graphics
{

be_abstractmetaclass_impl("Graphics", Primitive);

Primitive::Primitive(const RenderBackend* /*renderer*/)
{
}

Primitive::~Primitive()
{
}

}}
