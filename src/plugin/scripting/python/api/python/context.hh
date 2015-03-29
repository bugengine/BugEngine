/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHON_CONTEXT_HH_
#define BE_PYTHON_CONTEXT_HH_
/**************************************************************************************************/
#include    <python/stdafx.h>
#include    <bugengine/scriptengine.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/value.hh>
#include    <filesystem/file.script.hh>
#include    <plugin/plugin.hh>
#include    <python/pythonscript.script.hh>
#include    <pythonlib/pythonlib.hh>

namespace BugEngine { namespace Python
{

class be_api(PYTHON) Context : public ScriptEngine<PythonScript>
{
public:
    Context(const Plugin::Context& context, ref<PythonLibrary> library);
    ~Context();

 private:
    virtual void unload(Resource::Resource& handle) override;
    virtual void runBuffer(weak<const PythonScript> script, Resource::Resource& resource,
                           const minitl::Allocator::Block<u8>& buffer) override;
    virtual void reloadBuffer(weak<const PythonScript> script, Resource::Resource& resource,
                              const minitl::Allocator::Block<u8>& buffer) override;
    static void pythonInitialise();
private:
    ref<PythonLibrary>  m_library;
    PyThreadState*      m_pythonState;
};

}}

/**************************************************************************************************/
#endif
