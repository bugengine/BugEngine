/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
see LICENSE for detail */

#include    <pythonlib/stdafx.h>
#include    <pythonlib/pythonlib.hh>
#include    <string.h>

namespace BugEngine { namespace Python
{

static HMODULE getPythonModuleHandle()
{
    typedef BOOL (WINAPI *EnumProcessModulesFunc)(HANDLE hProcess, HMODULE *lphModule, DWORD cb, LPDWORD lpcbNeeded);
    EnumProcessModulesFunc EnumProcessModules = 0;
    HMODULE h = LoadLibraryA("Psapi.dll");
    if (h != 0)
    {
        EnumProcessModules = (EnumProcessModulesFunc)GetProcAddress(h, "EnumProcessModules");
    }
    if (!EnumProcessModules)
    {
        if (h) FreeLibrary(h);
        h = LoadLibraryA("kernel32.dll");
        EnumProcessModules = (EnumProcessModulesFunc)GetProcAddress(h, "K32EnumProcessModules ");
    }

    if (EnumProcessModules)
    {
        DWORD needed = 0;
        if (!(*EnumProcessModules)(GetCurrentProcess(), 0, 0, &needed))
        {
            be_error("Could not locate python: EnumProcessModules failed");
            return (HMODULE)NULL;
        }
        minitl::Allocator::Block<HMODULE> modules(Arena::temporary(), needed / sizeof(HMODULE));
        if (!(*EnumProcessModules)(GetCurrentProcess(), modules.data(), (DWORD)modules.byteCount(), &needed))
        {
            be_error("Could not locate python: EnumProcessModules failed");
            return (HMODULE)NULL;
        }
        FreeLibrary(h);

        for (u64 i = modules.count(); i > 0; --i)
        {
            char filename[1024];
            DWORD len = GetModuleFileNameA(modules[i-1], filename, sizeof(filename));
            if (len)
            {
                char *f = filename + len;
                while (f != filename && *(f-1) != '/' && *(f-1) != '\\')
                {
                    if (*f >= 'A' && *f <= 'Z')
                    {
                        *f = *f + 'a' - 'A';
                    }
                    --f;
                }
                if (strncmp(f, "python", 6) == 0)
                {
                    return modules[i-1];
                }
            }
            else
            {
                be_warning("GetModuleFileName failed on module %p" | modules[i]);
            }
        }
        be_error("Could not locate python: could not locate module with name Python");
        return NULL;
    }
    else
    {
        FreeLibrary(h);
        be_error("Could not locate python: could not locate procedure \"EnumProcessModules\" in Psapi.dll or Kernel32.dll");
        return NULL;
    }
}

PythonLibrary::PythonLibrary(const char* pythonLibraryName)
    :   m_pythonLibraryName(pythonLibraryName)
    ,   m_handle(m_pythonLibraryName
                    ? LoadLibraryA(minitl::format<1024u>("%s.dll") | m_pythonLibraryName)
                    : getPythonModuleHandle())
    ,   m_status(m_handle != 0)
    ,   m_api(1013)
    ,   m_version(0)
{
    if (!m_status)
    {
        be_error("unable to load library %s" | pythonLibraryName);
    }
    else
    {
#   define be_get_func_name_opt(f, dest)                                        \
        do {                                                                    \
            void* tmp = (void*)(                                \
                            GetProcAddress((HMODULE)m_handle, #f));             \
            if (tmp)                                                            \
                memcpy(&m_##dest, &tmp, sizeof(dest##Type));                    \
        } while(0)
#   define be_get_func_opt(f)                                                   \
        be_get_func_name_opt(f, f)
#   define be_get_func_name(f, dest)                                            \
        do {                                                                    \
            be_get_func_name_opt(f, dest);                                      \
            if (!m_##dest)                                                      \
            {                                                                   \
                be_error("could not locate function %s in module %s"            \
                            | #f                                                \
                            | (pythonLibraryName ? pythonLibraryName : "root"));\
                m_status = false;                                               \
            }                                                                   \
        } while(0)
#   define be_get_func(f)                                                       \
        be_get_func_name(f, f)

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
            be_get_func_name(PyImport_AppendInittab, PyImport_AppendInittab3);
            be_get_func_name(Py_SetPythonHome, Py_SetPythonHome3);
        }
        else
        {
            m_Py_InitModule4 = 0;
            m_Py_InitModule4_64 = 0;
            be_get_func_opt(Py_InitModule4);
            be_get_func_opt(Py_InitModule4_64);
            be_get_func_name(PyImport_AppendInittab, PyImport_AppendInittab2);
            be_get_func_name(Py_SetPythonHome, Py_SetPythonHome2);
        }
        if (m_version >= 32)
        {
            m_Py_CompileStringFlags = 0;
            be_get_func(Py_CompileStringExFlags);
        }
        else
        {
            m_Py_CompileStringExFlags = 0;
            be_get_func(Py_CompileStringFlags);
        }
        be_get_func(PyEval_EvalCodeEx);

        be_get_func(PyModule_AddObject);
        be_get_func(PyModule_GetDict);
        be_get_func(PyImport_AddModule);
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
        be_get_func(PyObject_IsTrue);
        be_get_func(PyCFunction_NewEx);
        be_get_func(PyType_Ready);
        be_get_func(PyType_GenericAlloc);
        be_get_func(PyType_GenericNew);
        be_get_func(PyList_New);
        be_get_func(PyList_Size);
        be_get_func(PyList_GetItem);
        be_get_func(PyList_SetItem);
        be_get_func(PyList_Insert);
        be_get_func(PyList_Append);
        be_get_func(PyList_GetSlice);
        be_get_func(PyList_SetSlice);
        be_get_func(PyTuple_New);
        be_get_func(PyTuple_Size);
        be_get_func(PyTuple_GetItem);
        be_get_func(PyTuple_SetItem);
        be_get_func(PyTuple_GetSlice);
        be_get_func(PyDict_New);
        be_get_func(PyDict_Size);
        be_get_func(PyDict_GetItem);
        be_get_func(PyDict_SetItem);
        be_get_func(PyDict_DelItem);
        be_get_func(PyDict_Next);
        be_get_func(PyDict_GetItemString);
        be_get_func(PyDict_SetItemString);
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
            be_get_func_name_opt(PyUnicode_FromString, PyUnicode_FromString);
            be_get_func_name_opt(PyUnicodeUCS2_FromString, PyUnicode_FromString);
            be_get_func_name_opt(PyUnicodeUCS4_FromString, PyUnicode_FromString);
            be_get_func_name_opt(PyUnicode_FromStringAndSize, PyUnicode_FromStringAndSize);
            be_get_func_name_opt(PyUnicodeUCS2_FromStringAndSize, PyUnicode_FromStringAndSize);
            be_get_func_name_opt(PyUnicodeUCS4_FromStringAndSize, PyUnicode_FromStringAndSize);
            be_get_func_name_opt(PyUnicode_FromFormat, PyUnicode_FromFormat);
            be_get_func_name_opt(PyUnicodeUCS2_FromFormat, PyUnicode_FromFormat);
            be_get_func_name_opt(PyUnicodeUCS4_FromFormat, PyUnicode_FromFormat);
            be_get_func_name_opt(PyUnicode_AsASCIIString, PyUnicode_AsASCIIString);
            be_get_func_name_opt(PyUnicodeUCS2_AsASCIIString, PyUnicode_AsASCIIString);
            be_get_func_name_opt(PyUnicodeUCS4_AsASCIIString, PyUnicode_AsASCIIString);
            be_get_func_name_opt(PyUnicode_AsUTF8String, PyUnicode_AsUTF8String);
            be_get_func_name_opt(PyUnicodeUCS2_AsUTF8String, PyUnicode_AsUTF8String);
            be_get_func_name_opt(PyUnicodeUCS4_AsUTF8String, PyUnicode_AsUTF8String);
            if (m_version >= 33)
                be_get_func(PyUnicode_AsUTF8);
            be_get_func(PyBytes_AsString);
        }
        be_get_func(PyBool_FromLong);
        if (m_version < 30)
        {
            be_get_func(PyInt_AsUnsignedLongMask);
            be_get_func(PyInt_FromLong);
        }
        be_get_func(PyLong_AsUnsignedLongLongMask);
        be_get_func(PyLong_FromUnsignedLongLong);
        be_get_func(PyFloat_FromDouble);
        be_get_func(PyFloat_AsDouble);
        be_get_func(PyErr_Print);
        be_get_func(PyErr_SetString);
        be_get_func(PyErr_Format);
        be_get_func(PyErr_BadArgument);
        be_get_func(PyBool_Type);
        be_get_func(PyFloat_Type);
        be_get_func(PyExc_Exception);
        be_get_func(PyExc_AttributeError);
        be_get_func(PyExc_ImportError);
        be_get_func(PyExc_IndexError);
        be_get_func(PyExc_TypeError);
        be_get_func(PySys_GetObject);
        be_get_func(PySys_SetObject);
#       undef be_get_fun
#       undef be_get_fun_opt
    }
}

PythonLibrary::~PythonLibrary()
{
    if (m_status && m_pythonLibraryName)
    {
        finalize();
    }
    if (m_handle && m_pythonLibraryName)
    {
        FreeLibrary((HMODULE)m_handle);
    }
}

void PythonLibrary::platformInitialize()
{
}

void PythonLibrary::setupPath()
{
    ifilename programPath = Environment::getEnvironment().getProgramPath();
    programPath.pop_back();
    (*m_PyRun_SimpleString)(minitl::format<4096>("import sys; sys.path.append(\"%s\")")
                                               | programPath.str().name);
}

}}
