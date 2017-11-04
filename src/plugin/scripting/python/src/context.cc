/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/context.hh>

namespace BugEngine { namespace Python
{

Context::Context(const Plugin::Context& context, ref<PythonLibrary> library)
    :   ScriptEngine<PythonScript>(Arena::python(), context.resourceManager)
    ,   m_library(library)
    ,   m_pythonState(library->createThread())
{
}

Context::~Context()
{
    m_library->destroyThread(m_pythonState);
}

void Context::unload(Resource::Resource& /*handle*/)
{
    PythonLibrary::ThreadLock lock(m_library, m_pythonState);
}

void Context::runBuffer(weak<const PythonScript> script, Resource::Resource& /*resource*/,
                        const minitl::Allocator::Block<u8>& block)
{
    runCode(reinterpret_cast<const char*>(block.begin()), script->getScriptName());
}

void Context::reloadBuffer(weak<const PythonScript> script, Resource::Resource& /*resource*/,
                           const minitl::Allocator::Block<u8>& block)
{
    runCode(reinterpret_cast<const char*>(block.begin()), script->getScriptName());
}

void Context::runCode(const char *buffer, const ifilename &filename)
{
    PythonLibrary::ThreadLock lock(m_library, m_pythonState);
    PyCodeObject* code = 0;
    if (m_library->m_Py_CompileStringExFlags)
    {
        code = (*m_library->m_Py_CompileStringExFlags)(buffer,
                                                       filename.str().name,
                                                       Py_file_input, NULL,
                                                       -1);
    }
    else
    {
        code = (*m_library->m_Py_CompileStringFlags)(buffer,
                                                     filename.str().name,
                                                     Py_file_input, NULL);
    }
    if (code)
    {
        PyObject* m = (*m_library->m_PyImport_AddModule)("__main__");
        PyObject* d = (*m_library->m_PyModule_GetDict)(m);
        PyObject* result = (*m_library->m_PyEval_EvalCodeEx)(code, d, d, NULL, 0,
                                                             NULL, 0, NULL, 0, NULL);
        if (!result)
        {
            (*m_library->m_PyErr_Print)();
        }
        else
        {
            Py_DECREF(result);
        }
        Py_DECREF(code);
    }
    else
    {
        (*m_library->m_PyErr_Print)();
    }
}

}}
