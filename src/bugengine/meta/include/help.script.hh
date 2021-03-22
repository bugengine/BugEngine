/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_HELP_SCRIPT_HH_
#define BE_META_HELP_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/classinfo.script.hh>
#include <bugengine/meta/engine/methodinfo.script.hh>
#include <bugengine/meta/engine/propertyinfo.script.hh>

namespace BugEngine {

void help(const Meta::Type& type);
void help(const Meta::Class& klass);
void help(const Meta::Property& property);
void help(const Meta::Method& method);
void help(const Meta::Method::Overload& overload);
void help(const Meta::Method::Parameter& parameter);
void help(const Meta::Value& v);

}  // namespace BugEngine

/**************************************************************************************************/
#endif
