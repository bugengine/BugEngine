/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_BUGENGINE_SCRIPTENGINE_HH_
#define BE_BUGENGINE_SCRIPTENGINE_HH_
/*****************************************************************************/
#include    <bugengine/script.script.hh>
#include    <resource/resourceloader.hh>

namespace BugEngine
{

template< typename T >
class ScriptEngine : public IResourceLoader
{
protected:
    minitl::Allocator&      m_scriptArena;
    weak<ResourceManager>   m_manager;
public:
    virtual ~ScriptEngine();
protected:
    ScriptEngine(minitl::Allocator& arena, weak<ResourceManager> manager);
    virtual void runBuffer(weak<const T> resource, const minitl::Allocator::Block<u8>& buffer) = 0;
    virtual void unloadScript(const ResourceHandle& handle);

private:
    virtual ResourceHandle load(weak<const Resource> script) override;
    virtual void unload(const ResourceHandle& handle) override;
    virtual void onTicketLoaded(weak<const Resource> resource, const minitl::Allocator::Block<u8>& buffer) override;
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); ::operator delete(memory); }
};

}

#include <bugengine/scriptengine.inl>

/*****************************************************************************/
#endif
