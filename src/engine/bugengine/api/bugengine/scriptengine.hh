/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_BUGENGINE_SCRIPTENGINE_HH_
#define BE_BUGENGINE_SCRIPTENGINE_HH_
/*****************************************************************************/
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
    virtual void unloadScript(Resource::Resource& resource);

private:
    virtual void load(weak<const Resource::Description> script, Resource::Resource& resource) override;
    virtual void unload(Resource::Resource& handle) override;
    virtual void onTicketLoaded(weak<const Resource::Description> script, Resource::Resource& resource, const minitl::Allocator::Block<u8>& buffer) override;
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); ::operator delete(memory); }
};

}

#include <bugengine/scriptengine.inl>

/*****************************************************************************/
#endif
