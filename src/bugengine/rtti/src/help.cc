/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti/stdafx.h>
#include <bugengine/rtti/classinfo.script.hh>
#include <bugengine/rtti/engine/methodinfo.script.hh>
#include <bugengine/rtti/engine/propertyinfo.script.hh>
#include <bugengine/rtti/tags/documentation.script.hh>
#include <bugengine/rtti/value.hh>
#include <zlib.h>

namespace BugEngine {

void help(const RTTI::Type& type)
{
    be_forceuse(type);
}

void help(const RTTI::Class& klass)
{
    be_info("%s" | klass.name);
    be_forceuse(klass);
}

void help(const RTTI::Property& property)
{
    be_info("%s" | property.name);
    be_forceuse(property);
}

void help(const RTTI::Method& method)
{
    be_info("%s" | method.name);
    be_forceuse(method);
}

void help(const RTTI::Method::Overload& overload)
{
    be_forceuse(overload);
}

void help(const RTTI::Method::Parameter& parameter)
{
    be_info("%s" | parameter.name);
    be_forceuse(parameter);
}

void help(const RTTI::Value& v)
{
    be_forceuse(v);
}

}  // namespace BugEngine
