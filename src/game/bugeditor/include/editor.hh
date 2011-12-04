/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_EDITOR_EDITOR_HH_
#define BE_EDITOR_EDITOR_HH_
/*****************************************************************************/
#include    <system/file/folder.script.hh>
#include    <bugengine/ieditor.hh>
#include    <bugengine/application.hh>

namespace BugEngine { namespace Editor
{

class Editor : public IEditor
{
private:
    weak<Application> const m_application;
public:
    Editor(weak<Application> application);
    ~Editor();
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); ::operator delete(memory); }
};

}}

/*****************************************************************************/
#endif
