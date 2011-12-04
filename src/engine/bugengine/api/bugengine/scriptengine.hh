/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_BUGENGINE_SCRIPTENGINE_HH_
#define BE_BUGENGINE_SCRIPTENGINE_HH_
/*****************************************************************************/
#include    <bugengine/script.script.hh>
#include    <system/resource/resourceloader.hh>

namespace BugEngine
{

class IScriptEngine : public minitl::pointer
{
public:
    virtual ~IScriptEngine() {}
    virtual void update() = 0;
};

template< typename T >
class ScriptEngine : public IScriptEngine
{
private:
    Allocator&                                                                      m_scriptArena;
    minitl::vector< minitl::pair< ref<const File::Ticket>, weak<const Script> > >   m_tickets;
protected:
    ScriptEngine(Allocator& arena);
public:
    virtual ~ScriptEngine();
    virtual void runBuffer(weak<const T> resource, const Allocator::Block<u8>& buffer) = 0;
private:
    ResourceHandle loadScript(weak<const T> script);
    void unloadScript(const ResourceHandle& handle);
public:
    virtual void update() override;
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); ::operator delete(memory); }
};

}

#include <bugengine/scriptengine.inl>

/*****************************************************************************/
#endif
