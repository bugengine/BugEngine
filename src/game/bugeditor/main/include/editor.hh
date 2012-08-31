/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_EDITOR_EDITOR_HH_
#define BE_EDITOR_EDITOR_HH_
/*****************************************************************************/
#include    <filesystem/folder.script.hh>
#include    <bugengine/application.hh>
#include    <resource/resourcemanager.hh>
#include    <plugin/plugin.hh>

namespace BugEngine
{
class Folder;
class Package;
class IKernelScheduler;
}

namespace BugEngine { namespace Editor
{

class Editor : public Application
{
private:
    Plugin<void>                const   m_renderer;
    Plugin<Resource::ILoader>   const   m_packageManager;
    Plugin<Resource::ILoader>   const   m_luaScripting;
    ref<const Package>          const   m_mainPackage;
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
