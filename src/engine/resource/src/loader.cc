/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <resource/stdafx.h>
#include    <resource/loader.hh>


namespace BugEngine { namespace Resource
{

static i_u32 s_nextLoaderId(i_u32::create(1));
ILoader::ILoader()
    :   m_id(s_nextLoaderId++)
{
}

ILoader::~ILoader()
{
}

}}
