/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHONLIB_CONTEXT_H_
#define BE_PYTHONLIB_CONTEXT_H_
/**************************************************************************************************/
#include    <pythonlib/stdafx.h>
#include    <bugengine/scriptengine.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/value.hh>
#include    <filesystem/file.script.hh>
#include    <plugin/plugin.hh>
#include    <pythonlib/pythonscript.script.hh>
#include    <pythonlib/pythonlib.hh>

namespace BugEngine { namespace Python
{

class PythonGlobalInterpreter;
class be_api(PYTHONLIB) Context : public ScriptEngine<PythonScript>
{
    friend class PythonGlobalInterpreter;
public:
    Context(const Plugin::Context& context, weak<PythonLibrary> library);
    ~Context();

 private:
    virtual void unload(Resource::Resource& handle) override;
    virtual void runBuffer(weak<const PythonScript> script, Resource::Resource& resource,
                           const minitl::Allocator::Block<u8>& buffer) override;
    virtual void reloadBuffer(weak<const PythonScript> script, Resource::Resource& resource,
                              const minitl::Allocator::Block<u8>& buffer) override;
    static void pythonInitialise();
private:
    weak<PythonLibrary> m_library;
    PyThreadState*      m_pythonState;
};

}}

/**************************************************************************************************/
#endif
