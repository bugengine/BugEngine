/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_EDITOR_EDITOR_HH_
#define BE_EDITOR_EDITOR_HH_
/**************************************************************************************************/
#include <stdafx.h>
#include <bugengine/application.hh>
#include <bugengine/filesystem/folder.script.hh>
#include <bugengine/plugin/plugin.hh>
#include <bugengine/resource/resourcemanager.hh>

namespace BugEngine {
class Folder;
class Package;
class IKernelScheduler;
}  // namespace BugEngine

namespace BugEngine { namespace Editor {

class Editor : public Application
{
private:
    Plugin::Plugin< void > const              m_renderer;
    Plugin::Plugin< Resource::ILoader > const m_packageManager;
    Plugin::Plugin< Resource::ILoader > const m_luaScripting;
    ref< const Package > const                m_mainPackage;

public:
    Editor(const Plugin::Context& context);
    ~Editor();
};

}}  // namespace BugEngine::Editor

/**************************************************************************************************/
#endif
