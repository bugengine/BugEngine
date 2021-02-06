/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_HELP_SCRIPT_HH_
#define BE_RTTI_HELP_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/rtti/stdafx.h>
#include <bugengine/rtti/classinfo.script.hh>
#include <bugengine/rtti/engine/methodinfo.script.hh>
#include <bugengine/rtti/engine/propertyinfo.script.hh>

namespace BugEngine {

void help(const RTTI::Type& type);
void help(const RTTI::Class& klass);
void help(const RTTI::Property& property);
void help(const RTTI::Method& method);
void help(const RTTI::Method::Overload& overload);
void help(const RTTI::Method::Parameter& parameter);
void help(const RTTI::Value& v);

}  // namespace BugEngine

/**************************************************************************************************/
#endif
