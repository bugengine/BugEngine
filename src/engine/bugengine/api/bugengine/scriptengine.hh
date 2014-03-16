/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_BUGENGINE_SCRIPTENGINE_HH_
#define BE_BUGENGINE_SCRIPTENGINE_HH_
/**************************************************************************************************/
#include    <bugengine/stdafx.h>
#include    <bugengine/script.script.hh>
#include    <resource/loader.hh>

namespace BugEngine
{

template< typename T >
class ScriptEngine : public Resource::ILoader
{
protected:
    minitl::Allocator&              m_scriptArena;
    weak<Resource::ResourceManager> m_manager;
public:
    virtual ~ScriptEngine();
protected:
    ScriptEngine(minitl::Allocator& arena, weak<Resource::ResourceManager> manager);
    virtual void runBuffer(weak<const T> script, Resource::Resource& resource, const minitl::Allocator::Block<u8>& buffer) = 0;
    virtual void reloadBuffer(weak<const T> script, Resource::Resource& resource, const minitl::Allocator::Block<u8>& buffer) = 0;

private:
    virtual void load(weak<const Resource::Description> script, Resource::Resource& resource) override;
    virtual void reload(weak<const Resource::Description> oldScript, weak<const Resource::Description> newScript, Resource::Resource& resource) override;
    virtual void onTicketLoaded(weak<const Resource::Description> script, Resource::Resource& resource, const minitl::Allocator::Block<u8>& buffer, ILoader::LoadType type) override;
};

}

#include    <bugengine/scriptengine.inl>

/**************************************************************************************************/
#endif
