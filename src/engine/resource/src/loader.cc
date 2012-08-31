/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <resource/stdafx.h>
#include    <resource/loader.hh>


namespace BugEngine { namespace Resource
{

static i_u32 s_nextLoaderId(i_u32::One);
ILoader::ILoader()
    :   m_id(s_nextLoaderId++)
{
}

ILoader::~ILoader()
{
}

}}
