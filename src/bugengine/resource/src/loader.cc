/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/resource/stdafx.h>
#include <bugengine/resource/loader.hh>

namespace BugEngine { namespace Resource {

static i_u32 s_nextLoaderId(i_u32::create(1));
ILoader::ILoader() : m_id(s_nextLoaderId++)
{
}

ILoader::~ILoader()
{
}

}}  // namespace BugEngine::Resource
