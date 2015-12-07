/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHONLIB_PYTHONLIB_H_
#define BE_PYTHONLIB_PYTHONLIB_H_
/**************************************************************************************************/
#include    <pythonlib/stdafx.h>
#include    <pythonlib/pythontypes.hh>
#include    <plugin/plugin.hh>
#include    <core/threads/threadlocal.hh>

namespace BugEngine { namespace Python
{

class be_api(PYTHONLIB) PythonLibrary : public minitl::refcountable
{
public:
    PythonLibrary(const char* pythonLibraryName);
    ~PythonLibrary();

    operator const void* () const   { return (const void*)m_status; }
    bool operator!() const          { return !m_status; }


    void initialize();
    void finalize();

    int getVersion() const;
    int getApi() const;

    PyThreadState* createThread();
    void destroyThread(PyThreadState* threadState);

    struct be_api(PYTHONLIB) ThreadLock
    {
        ThreadLock(weak<PythonLibrary> library, PyThreadState* thread);
        ~ThreadLock();
    private:
        weak<PythonLibrary> m_library;
        PyThreadState*      m_thread;
    };

private:
    void platformInitialize();
    void setupPath();
private:
    friend struct ThreadLock;
private:
    const char*                             m_pythonLibraryName;
    void*                                   m_handle;
    bool                                    m_status;
    int                                     m_api;
    int                                     m_version;
    PyThreadState*                          m_mainThread;
private: // friend ThreadLock
    Py_SetPythonHomeType                    m_Py_SetPythonHome;
    Py_InitializeExType                     m_Py_InitializeEx;
    Py_FinalizeType                         m_Py_Finalize;
    Py_NewInterpreterType                   m_Py_NewInterpreter;
    Py_EndInterpreterType                   m_Py_EndInterpreter;
    Py_GetPathType                          m_Py_GetPath;
    Py_GetVersionType                       m_Py_GetVersion;

    PyEval_InitThreadsType                  m_PyEval_InitThreads;
    PyEval_SaveThreadType                   m_PyEval_SaveThread;
    PyEval_AcquireThreadType                m_PyEval_AcquireThread;
    PyEval_ReleaseThreadType                m_PyEval_ReleaseThread;
    PyEval_ReleaseLockType                  m_PyEval_ReleaseLock;
public:
    PyRun_SimpleStringType                  m_PyRun_SimpleString;
    Py_CompileStringFlagsType               m_Py_CompileStringFlags;
    Py_CompileStringExFlagsType             m_Py_CompileStringExFlags;
    PyEval_EvalCodeExType                   m_PyEval_EvalCodeEx;
    Py_InitModule4Type                      m_Py_InitModule4;
    Py_InitModule4_64Type                   m_Py_InitModule4_64;
    PyModule_Create2Type                    m_PyModule_Create2;
    PyModule_AddObjectType                  m_PyModule_AddObject;
    PyModule_GetDictType                    m_PyModule_GetDict;
    PyImport_AddModuleType                  m_PyImport_AddModule;
    PyImport_AppendInittab2Type             m_PyImport_AppendInittab2;
    PyImport_AppendInittab3Type             m_PyImport_AppendInittab3;
    _Py_NoneStructType                      m__Py_NoneStruct;

    PyObject_SetAttrStringType              m_PyObject_SetAttrString;
    PyObject_GetAttrStringType              m_PyObject_GetAttrString;
    _PyArg_ParseTuple_SizeTType             m__PyArg_ParseTuple_SizeT;
    _PyArg_ParseTupleAndKeywords_SizeTType  m__PyArg_ParseTupleAndKeywords_SizeT;
    PyObject_IsTrueType                     m_PyObject_IsTrue;
    PyType_GenericAllocType                 m_PyType_GenericAlloc;
    PyType_GenericNewType                   m_PyType_GenericNew;
    PyType_ReadyType                        m_PyType_Ready;

    PyList_NewType                          m_PyList_New;
    PyList_SizeType                         m_PyList_Size;
    PyList_GetItemType                      m_PyList_GetItem;
    PyList_SetItemType                      m_PyList_SetItem;
    PyList_InsertType                       m_PyList_Insert;
    PyList_AppendType                       m_PyList_Append;
    PyList_GetSliceType                     m_PyList_GetSlice;
    PyList_SetSliceType                     m_PyList_SetSlice;

    PyTuple_NewType                         m_PyTuple_New;
    PyTuple_SizeType                        m_PyTuple_Size;
    PyTuple_GetItemType                     m_PyTuple_GetItem;
    PyTuple_SetItemType                     m_PyTuple_SetItem;
    PyTuple_GetSliceType                    m_PyTuple_GetSlice;
    
    PyDict_NewType                          m_PyDict_New;
    PyDict_SizeType                         m_PyDict_Size;
    PyDict_GetItemType                      m_PyDict_GetItem;
    PyDict_SetItemType                      m_PyDict_SetItem;
    PyDict_DelItemType                      m_PyDict_DelItem;
    PyDict_NextType                         m_PyDict_Next;
    PyDict_GetItemStringType                m_PyDict_GetItemString;
    PyDict_SetItemStringType                m_PyDict_SetItemString;

    PyString_FromStringType                 m_PyString_FromString;
    PyString_FromStringAndSizeType          m_PyString_FromStringAndSize;
    PyString_FromFormatType                 m_PyString_FromFormat;
    PyString_SizeType                       m_PyString_Size;
    PyString_AsStringType                   m_PyString_AsString;
    PyString_AsStringAndSizeType            m_PyString_AsStringAndSize;

    PyUnicode_FromStringType                m_PyUnicode_FromString;
    PyUnicode_FromStringAndSizeType         m_PyUnicode_FromStringAndSize;
    PyUnicode_FromFormatType                m_PyUnicode_FromFormat;
    PyUnicode_AsUTF8Type                    m_PyUnicode_AsUTF8;
    PyUnicode_AsUTF8StringType              m_PyUnicode_AsUTF8String;
    PyUnicode_AsASCIIStringType             m_PyUnicode_AsASCIIString;
    PyBytes_AsStringType                    m_PyBytes_AsString;

    PyBool_FromLongType                     m_PyBool_FromLong;
    PyLong_FromUnsignedLongLongType         m_PyLong_FromUnsignedLongLong;
    PyLong_AsUnsignedLongLongMaskType       m_PyLong_AsUnsignedLongLongMask;
    PyInt_FromLongType                      m_PyInt_FromLong;
    PyInt_AsUnsignedLongMaskType            m_PyInt_AsUnsignedLongMask;
    PyFloat_FromDoubleType                  m_PyFloat_FromDouble;
    PyFloat_AsDoubleType                    m_PyFloat_AsDouble;

    PyErr_PrintType                         m_PyErr_Print;
    PyErr_SetStringType                     m_PyErr_SetString;
    PyErr_FormatType                        m_PyErr_Format;
    PyErr_BadArgumentType                   m_PyErr_BadArgument;

    PyFloat_TypeType                        m_PyFloat_Type;
    PyExc_ExceptionType                     m_PyExc_Exception;
    PyExc_AttributeErrorType                m_PyExc_AttributeError;
    PyExc_ImportErrorType                   m_PyExc_ImportError;
    PyExc_IndexErrorType                    m_PyExc_IndexError;
    PyExc_TypeErrorType                     m_PyExc_TypeError;

    PySys_GetObjectType                     m_PySys_GetObject;
    PySys_SetObjectType                     m_PySys_SetObject;
};

extern tls<PythonLibrary> s_library;

be_api(PYTHONLIB) PyObject* init2_py_bugengine(bool registerLog);
be_api(PYTHONLIB) PyObject* init3_py_bugengine(bool registerLog);
be_api(PYTHONLIB) ref<PythonLibrary> loadPython(const char* pythonPath);
be_api(PYTHONLIB) void setCurrentContext(weak<PythonLibrary> library);
be_api(PYTHONLIB) void clearCurrentContext();

}}

/**************************************************************************************************/
#endif
