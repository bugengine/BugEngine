/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/classinfo.script.hh>
#include <bugengine/meta/engine/methodinfo.script.hh>
#include <bugengine/meta/engine/propertyinfo.script.hh>
#include <bugengine/meta/tags/documentation.script.hh>
#include <bugengine/meta/value.hh>
#include <zlib.h>

namespace BugEngine {

void help(const Meta::Type& type)
{
    be_forceuse(type);
}

void help(const Meta::Class& klass)
{
    be_info("%s" | klass.name);
    be_forceuse(klass);
}

void help(const Meta::Property& property)
{
    be_info("%s" | property.name);
    be_forceuse(property);
}

void help(const Meta::Method& method)
{
    be_info("%s" | method.name);
    be_forceuse(method);
}

void help(const Meta::Method::Overload& overload)
{
    be_forceuse(overload);
}

void help(const Meta::Method::Parameter& parameter)
{
    be_info("%s" | parameter.name);
    be_forceuse(parameter);
}

void help(const Meta::Value& v)
{
    be_forceuse(v);
}

}  // namespace BugEngine
