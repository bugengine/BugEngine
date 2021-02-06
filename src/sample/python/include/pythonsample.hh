/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SAMPLES_PYTHON_PYTHONSAMPLE_HH_
#define BE_SAMPLES_PYTHON_PYTHONSAMPLE_HH_
/**************************************************************************************************/
#include <stdafx.h>
#include <bugengine/application.hh>
#include <bugengine/filesystem/folder.script.hh>
#include <bugengine/plugin.scripting.package/package.script.hh>
#include <bugengine/plugin/plugin.hh>
#include <bugengine/resource/resourcemanager.hh>

namespace BugEngine {

class PythonSample : public Application
{
private:
    Plugin::Plugin< Resource::ILoader > const m_packageManager;
    Plugin::Plugin< Resource::ILoader > const m_pythonManager;
    ref< const Package > const                m_mainPackage;

public:
    PythonSample(const Plugin::Context& context);
    ~PythonSample();
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
