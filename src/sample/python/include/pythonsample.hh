/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SAMPLES_PYTHON_PYTHONSAMPLE_HH_
#define BE_SAMPLES_PYTHON_PYTHONSAMPLE_HH_
/**************************************************************************************************/
#include    <stdafx.h>
#include    <filesystem/folder.script.hh>
#include    <bugengine/application.hh>
#include    <resource/resourcemanager.hh>
#include    <plugin/plugin.hh>
#include    <package/package.script.hh>

namespace BugEngine
{

class PythonSample : public Application
{
private:
    Plugin::Plugin<Resource::ILoader>   const   m_packageManager;
    Plugin::Plugin<Resource::ILoader>   const   m_pythonManager;
    ref<const Package>                  const   m_mainPackage;
public:
    PythonSample(const Plugin::Context& context);
    ~PythonSample();
};

}

/**************************************************************************************************/
#endif
