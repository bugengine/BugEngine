/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_EDITOR_EDITOR_HH_
#define BE_EDITOR_EDITOR_HH_
/*****************************************************************************/
#include    <system/file/folder.script.hh>
#include    <bugengine/application.hh>
#include    <system/resource/resourcemanager.hh>
#include    <system/plugin.hh>

namespace BugEngine { namespace Editor
{

class Editor : public Application
{
private:
    scoped<ResourceManager> const   m_resourceManager;
    PluginContext           const   m_pluginContext;
    Plugin<IResourceLoader> const   m_packageBuilder;
public:
    Editor(const PluginContext& context);
    ~Editor();
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); ::operator delete(memory); }
};

}}

/*****************************************************************************/
#endif
