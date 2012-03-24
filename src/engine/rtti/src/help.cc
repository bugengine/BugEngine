/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/tags/documentation.script.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <zlib.h>

namespace BugEngine
{

void help(const RTTI::Class& klass)
{
    be_forceuse(klass);
}

void help(const RTTI::Property& property)
{
    be_forceuse(property);
}

void help(const RTTI::Method& method)
{
    be_forceuse(method);
}

void help(const RTTI::Method::Overload& overload)
{
    be_forceuse(overload);
}

void help(const RTTI::Method::Overload::Parameter& parameter)
{
    be_forceuse(parameter);
}


void help(const Value& v)
{
    be_forceuse(v);
}

}

