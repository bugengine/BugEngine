/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <text/stdafx.h>
#include    <textmanager.hh>

namespace BugEngine
{

TextManager::TextManager()
{
}

TextManager::~TextManager()
{
}

void TextManager::load(weak<const Resource::Description> /*description*/, Resource::Resource& /*resource*/)
{
    be_info("loading text");
}

void TextManager::reload(weak<const Resource::Description> /*oldDescription*/, weak<const Resource::Description> /*newDescription*/, Resource::Resource& /*resource*/)
{
    be_info("reloading text");
}

void TextManager::unload(Resource::Resource& /*resource*/)
{
    be_info("unloading text");
}

}

