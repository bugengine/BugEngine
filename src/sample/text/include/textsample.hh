/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SAMPLES_TEXT_KERNELSAMPLE_HH_
#define BE_SAMPLES_TEXT_KERNELSAMPLE_HH_
/*****************************************************************************/
#include    <filesystem/folder.script.hh>
#include    <bugengine/application.hh>
#include    <resource/resourcemanager.hh>
#include    <scheduler/kernel/kernel.script.hh>
#include    <plugin/plugin.hh>
#include    <package/package.script.hh>

namespace BugEngine
{

class TextSample : public Application
{
private:
    Plugin::Plugin<Resource::ILoader>   const   m_packageManager;
    Plugin::Plugin<minitl::pointer>     const   m_textManager;
    ref<const Package>                  const   m_mainPackage;
public:
    TextSample(const Plugin::Context& context);
    ~TextSample();
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); ::operator delete(memory); }
};

}

/*****************************************************************************/
#endif

