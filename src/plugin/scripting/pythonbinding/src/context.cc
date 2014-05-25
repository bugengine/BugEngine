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

class PythonGlobalInterpreter
{
public:
    PythonGlobalInterpreter()
    {
        Context::pythonInitialise();
        Py_InitializeEx(0);
        PyEval_InitThreads();
        m_mainThread = PyEval_SaveThread();
    }
    ~PythonGlobalInterpreter()
    {
        PyEval_AcquireThread(m_mainThread);
        Py_Finalize();
    }
    PyThreadState* createInterpreter()
    {
        PyEval_AcquireThread(m_mainThread);
        PyThreadState* result = Py_NewInterpreter();
        PyEval_ReleaseThread(result);
        return result;
    }
    void destroyInterpreter(PyThreadState* state)
    {
        PyEval_AcquireThread(state);
        Py_EndInterpreter(state);
        PyEval_ReleaseLock();
    }
private:
    PyThreadState*  m_mainThread;
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
    PyEval_AcquireThread(m_pythonState);
    PyEval_ReleaseThread(m_pythonState);
}

void Context::runBuffer(weak<const PythonScript> /*script*/, Resource::Resource& /*resource*/,
                        const minitl::Allocator::Block<u8>& block)
{
    PyEval_AcquireThread(m_pythonState);
    PyRun_SimpleString((const char*)block.begin());
    PyEval_ReleaseThread(m_pythonState);
}

void Context::reloadBuffer(weak<const PythonScript> /*script*/, Resource::Resource& /*resource*/,
                           const minitl::Allocator::Block<u8>& block)
{
    PyEval_AcquireThread(m_pythonState);
    PyRun_SimpleString((const char*)block.begin());
    PyEval_ReleaseThread(m_pythonState);
}

}}
