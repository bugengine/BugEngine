/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHON27_CONTEXT_H_
#define BE_PYTHON27_CONTEXT_H_
/**************************************************************************************************/
#include    <stdafx.h>
#include    <bugengine/scriptengine.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/value.hh>
#include    <filesystem/file.script.hh>
#include    <plugin/plugin.hh>
#include    <pythonscript.script.hh>

namespace BugEngine { namespace Python27
{

class Context : public ScriptEngine<PythonScript>
{
public:
    Context(const Plugin::Context& context);
    ~Context();

 private:
    virtual void unload(Resource::Resource& handle) override;
    virtual void runBuffer(weak<const PythonScript> script, Resource::Resource& resource, const minitl::Allocator::Block<u8>& buffer) override;
    virtual void reloadBuffer(weak<const PythonScript> script, Resource::Resource& resource, const minitl::Allocator::Block<u8>& buffer) override;

private:
    PyThreadState* m_pythonState;
};

}}

/**************************************************************************************************/
#endif
