/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_HELP_SCRIPT_HH_
#define BE_RTTI_HELP_SCRIPT_HH_
/*****************************************************************************/
#include <rtti/engine/methodinfo.script.hh>
#include <rtti/engine/propertyinfo.script.hh>
#include <rtti/classinfo.script.hh>

namespace BugEngine
{

void help(const RTTI::Type& type);
void help(const RTTI::Class& klass);
void help(const RTTI::Property& klass);
void help(const RTTI::Method& klass);
void help(const RTTI::Method::Overload& klass);
void help(const RTTI::Method::Overload::Parameter& klass);
void help(const RTTI::Value& v);

}

/*****************************************************************************/
#endif
