/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/objects/shaders/method.script.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{

Method::Method(minitl::vector< ref<Param> > output)
    :   output(output)
{
}

Method::~Method()
{
}

void Method::buildSource(IShaderBuilder& /*stream*/) const
{
}

}}}
