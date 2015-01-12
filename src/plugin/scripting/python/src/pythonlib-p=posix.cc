/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonlib.hh>

#include    <dlfcn.h>

namespace BugEngine { namespace Python
{

PythonLibrary::PythonLibrary(const char* pythonLibraryName)
    :   m_pythonLibraryName(pythonLibraryName)
    ,   m_handle(m_pythonLibraryName
                    ?   dlopen(minitl::format<1024u>("lib%s.so") | m_pythonLibraryName,
                        RTLD_LAZY | RTLD_GLOBAL)
                    :   0 /*RTLD_DEFAULT*/)
    ,   m_status(dlerror() == 0)
    ,   m_api(1013)
    ,   m_version(0)
{
    if (!m_status)
    {
        be_error("unable to load library %s: %s" | pythonLibraryName | dlerror());
    }
    else
    {
#       define be_get_func_opt(f)                                               \
            void* tmp##f = dlsym(m_handle, #f);                                 \
            memcpy(&m_##f, &tmp##f, sizeof(f##Type));
#       define be_get_func(f)                                                   \
            be_get_func_opt(f)                                                  \
            if (!m_##f)                                                         \
            {                                                                   \
                be_error("could not locate function %s in module %s"            \
                            | #f                                                \
                            | (pythonLibraryName ? pythonLibraryName : "root"));\
                m_status = false;                                               \
            }
        
        be_get_func(Py_SetPythonHome);
        be_get_func(Py_InitializeEx);
        be_get_func(Py_Finalize);
        be_get_func(Py_NewInterpreter);
        be_get_func(Py_EndInterpreter);
        be_get_func(Py_GetPath);
        be_get_func(Py_GetVersion);
        const char* version = m_Py_GetVersion();
        m_version = (version[0]-'0')*10 + (version[2]-'0');
        if (m_version >= 30)
        {
            be_get_func(PyModule_Create2);
        }
        else
        {
            be_get_func_opt(Py_InitModule4);
            be_get_func_opt(Py_InitModule4_64);
        }
        be_get_func(PyModule_AddObject);
        be_get_func(PyEval_InitThreads);
        be_get_func(PyEval_SaveThread);
        be_get_func(PyEval_AcquireThread);
        be_get_func(PyEval_ReleaseThread);
        be_get_func(PyEval_ReleaseLock);
        be_get_func(PyRun_SimpleString);
        be_get_func(_Py_NoneStruct);
        be_get_func(PyObject_SetAttrString);
        be_get_func(PyObject_GetAttrString);
        be_get_func(_PyArg_ParseTuple_SizeT);
        be_get_func(_PyArg_ParseTupleAndKeywords_SizeT);
        be_get_func(PyType_Ready);
        be_get_func(PyList_New);
        be_get_func(PyList_Size);
        be_get_func(PyList_GetItem);
        be_get_func(PyList_SetItem);
        be_get_func(PyList_Insert);
        be_get_func(PyList_Append);
        be_get_func(PyList_GetSlice);
        be_get_func(PyList_SetSlice);
        if (m_version < 30)
        {
            be_get_func(PyString_FromString);
            be_get_func(PyString_FromStringAndSize);
            be_get_func(PyString_FromFormat);
            be_get_func(PyString_Size);
            be_get_func(PyString_AsString);
            be_get_func(PyString_AsStringAndSize);
        }
        else
        {
            be_get_func(PyUnicode_FromString);
            be_get_func(PyUnicode_FromStringAndSize);
            be_get_func(PyUnicode_FromFormat);
        }
        be_get_func(PyBool_FromLong);
        be_get_func(PyLong_FromLong);
        be_get_func(PyLong_FromSsize_t);
        be_get_func(PyFloat_FromDouble);
        be_get_func(PyErr_SetString);
        be_get_func(PyErr_Format);
        be_get_func(PyExc_Exception);
        be_get_func(PyExc_AttributeError);
        be_get_func(PyExc_ImportError);
        be_get_func(PyExc_IndexError);
#       undef be_get_fun
#       undef be_get_fun_opt
        if (m_status && pythonLibraryName)
        {
            initialize();
        }
    }
}

PythonLibrary::~PythonLibrary()
{
    if (m_status && m_pythonLibraryName)
    {
        finalize();
    }
    if (m_handle)
    {
        dlclose(m_handle);
    }
}

void PythonLibrary::platformInitialize()
{
}

void PythonLibrary::setupPath()
{
    const char* programPath = Environment::getEnvironment().getProgramPath();
    size_t programPathLength = strlen(programPath);
    minitl::Allocator::Block<char> pythonHome(Arena::python(),
                                              programPathLength + 1, 1);
    strcpy(pythonHome.begin(), programPath);
    char* path = pythonHome.begin();
    while (*path) path++;
    while (*path != '/' && *path != '\\' && path != pythonHome.begin()) path--;
    *path = 0;
    while (*path != '/' && *path != '\\' && path != pythonHome.begin()) path--;
    *path = 0;
    (*m_PyRun_SimpleString)(minitl::format<4096>("import sys; sys.path.append(\"%s/lib/\")") | pythonHome.data());
}

}}
