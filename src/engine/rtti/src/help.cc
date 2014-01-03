/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/tags/documentation.script.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/value.hh>
#include    <zlib.h>

namespace BugEngine
{

void help(const RTTI::Type& type)
{
    be_forceuse(type);
}

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


void help(const RTTI::Value& v)
{
    be_forceuse(v);
}

}

