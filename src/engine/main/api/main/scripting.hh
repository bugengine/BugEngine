/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MAIN_SCRIPTING_H_
#define BE_MAIN_SCRIPTING_H_
/*****************************************************************************/
#include    <system/file/file.script.hh>

namespace BugEngine
{

namespace RTTI
{
struct ClassInfo;
}

class File;

class BE_EXPORT IScriptEngine : public minitl::pointer
{
private:
    minitl::vector< ref<const File::Ticket> >   m_tickets;
protected:
    IScriptEngine(Allocator& arena);
public:
    virtual void addNamespace(istring name, const RTTI::ClassInfo* classinfo) = 0;
    virtual void runBuffer(const Allocator::Block<u8>& buffer) = 0;

    void loadFile(weak<const File>, Allocator& arena);
    virtual void update();

    virtual ~IScriptEngine();
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); ::operator delete(memory); }
};

}

/*****************************************************************************/
#endif
