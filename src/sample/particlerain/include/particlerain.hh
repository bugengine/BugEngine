/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SAMPLES_PARTICLERAIN_PARTICLERAIN_HH_
#define BE_SAMPLES_PARTICLERAIN_PARTICLERAIN_HH_
/**************************************************************************************************/
#include    <stdafx.h>
#include    <filesystem/folder.script.hh>
#include    <bugengine/application.hh>
#include    <resource/resourcemanager.hh>
#include    <scheduler/kernel/kernel.script.hh>
#include    <plugin/plugin.hh>
#include    <package/package.script.hh>

namespace BugEngine { namespace ParticleRain
{

class ParticleRain : public Application
{
private:
    Plugin::Plugin<Resource::ILoader>   const   m_packageManager;
    Plugin::Plugin<Resource::ILoader>   const   m_computeModule;
    ref<const Package>                  const   m_mainPackage;
public:
    ParticleRain(const Plugin::Context& context);
    ~ParticleRain();
};

}}

/**************************************************************************************************/
#endif

