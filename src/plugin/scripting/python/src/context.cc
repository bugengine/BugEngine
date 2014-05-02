/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>

#include    <context.hh>

namespace BugEngine
{

namespace Arena
{

static minitl::Allocator& python27()
{
    return script();
}

}

namespace Python
{

void platformInit();

class PythonGlobalInterpreter
{
public:
    PythonGlobalInterpreter()
    {
        Py_NoSiteFlag = 1;
        Py_InteractiveFlag = 0;
        Py_InitializeEx(0);
        be_info("python %s" | Py_GetVersion());
        PyEval_InitThreads();
        platformInit();
        PyEval_ReleaseLock();
    }
    ~PythonGlobalInterpreter()
    {
        Py_Finalize();
    }
    PyThreadState* createInterpreter()
    {
        PyEval_AcquireLock();
        PyThreadState* result = Py_NewInterpreter();
        PyEval_ReleaseLock();
        return result;
    }
    void destroyInterpreter(PyThreadState* state)
    {
        PyEval_RestoreThread(state);
        Py_EndInterpreter(state);
        PyEval_ReleaseLock();
    }
};

static PythonGlobalInterpreter s_interpreter;

Context::Context(const Plugin::Context& context)
    :   ScriptEngine<PythonScript>(Arena::python27(), context.resourceManager)
    ,   m_pythonState(s_interpreter.createInterpreter())
{
}

Context::~Context()
{
    s_interpreter.destroyInterpreter(m_pythonState);
}

void Context::unload(Resource::Resource& /*handle*/)
{
    PyEval_RestoreThread(m_pythonState);
    PyEval_SaveThread();
}

void Context::runBuffer(weak<const PythonScript> /*script*/, Resource::Resource& /*resource*/, const minitl::Allocator::Block<u8>& block)
{
    PyEval_RestoreThread(m_pythonState);
    PyRun_SimpleString((const char*)block.begin());
    PyEval_SaveThread();
}

void Context::reloadBuffer(weak<const PythonScript> /*script*/, Resource::Resource& /*resource*/, const minitl::Allocator::Block<u8>& block)
{
    PyEval_RestoreThread(m_pythonState);
    PyRun_SimpleString((const char*)block.begin());
    PyEval_SaveThread();
}

}}
