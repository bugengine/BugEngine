/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_PYTHON_CONTEXT_HH_
#define BE_PYTHON_CONTEXT_HH_
/**************************************************************************************************/
#include <bugengine/plugin.scripting.python/stdafx.h>
#include <bugengine/filesystem/file.script.hh>
#include <bugengine/meta/classinfo.script.hh>
#include <bugengine/meta/value.hh>
#include <bugengine/plugin.scripting.python/pythonscript.script.hh>
#include <bugengine/plugin.scripting.pythonlib/pythonlib.hh>
#include <bugengine/plugin/plugin.hh>
#include <bugengine/scriptengine.hh>

namespace BugEngine { namespace Python {

class be_api(PYTHON) Context : public ScriptEngine< PythonScript >
{
public:
    Context(const Plugin::Context& context, ref< PythonLibrary > library);
    ~Context();

private:
    virtual void unload(Resource::Resource & handle) override;
    virtual void runBuffer(weak< const PythonScript > script, Resource::Resource & resource,
                           const minitl::Allocator::Block< u8 >& buffer) override;
    virtual void reloadBuffer(weak< const PythonScript > script, Resource::Resource & resource,
                              const minitl::Allocator::Block< u8 >& buffer) override;
    static void  pythonInitialise();

private:
    void runCode(const char* buffer, const ifilename& filename);

private:
    ref< PythonLibrary > m_library;
    PyThreadState*       m_pythonState;
};

}}  // namespace BugEngine::Python

/**************************************************************************************************/
#endif
