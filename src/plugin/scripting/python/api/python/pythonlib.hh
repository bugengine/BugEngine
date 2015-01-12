/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHON_PYTHONLIB_H_
#define BE_PYTHON_PYTHONLIB_H_
/**************************************************************************************************/
#include    <python/stdafx.h>
#include    <python/pythontypes.hh>
#include    <plugin/plugin.hh>

namespace BugEngine { namespace Python
{

class be_api(PYTHON) PythonLibrary : public minitl::refcountable
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

    struct ThreadLock
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
    Py_InitModule4Type                      m_Py_InitModule4;
    Py_InitModule4_64Type                   m_Py_InitModule4_64;
    PyModule_Create2Type                    m_PyModule_Create2;
    PyModule_AddObjectType                  m_PyModule_AddObject;
    _Py_NoneStructType                      m__Py_NoneStruct;

    PyObject_SetAttrStringType              m_PyObject_SetAttrString;
    PyObject_GetAttrStringType              m_PyObject_GetAttrString;
    _PyArg_ParseTuple_SizeTType             m__PyArg_ParseTuple_SizeT;
    _PyArg_ParseTupleAndKeywords_SizeTType  m__PyArg_ParseTupleAndKeywords_SizeT;
    PyType_ReadyType                        m_PyType_Ready;

    PyList_NewType                          m_PyList_New;
    PyList_SizeType                         m_PyList_Size;
    PyList_GetItemType                      m_PyList_GetItem;
    PyList_SetItemType                      m_PyList_SetItem;
    PyList_InsertType                       m_PyList_Insert;
    PyList_AppendType                       m_PyList_Append;
    PyList_GetSliceType                     m_PyList_GetSlice;
    PyList_SetSliceType                     m_PyList_SetSlice;

    PyString_FromStringType                 m_PyString_FromString;
    PyString_FromStringAndSizeType          m_PyString_FromStringAndSize;
    PyString_FromFormatType                 m_PyString_FromFormat;
    PyString_SizeType                       m_PyString_Size;
    PyString_AsStringType                   m_PyString_AsString;
    PyString_AsStringAndSizeType            m_PyString_AsStringAndSize;

    PyUnicode_FromStringType                m_PyUnicode_FromString;
    PyUnicode_FromStringAndSizeType         m_PyUnicode_FromStringAndSize;
    PyUnicode_FromFormatType                m_PyUnicode_FromFormat;

    PyBool_FromLongType                     m_PyBool_FromLong;
    PyLong_FromLongType                     m_PyLong_FromLong;
    PyLong_FromSsize_tType                  m_PyLong_FromSsize_t;
    PyFloat_FromDoubleType                  m_PyFloat_FromDouble;

    PyErr_SetStringType                     m_PyErr_SetString;
    PyErr_FormatType                        m_PyErr_Format;
    
    PyExc_ExceptionType                     m_PyExc_Exception;
    PyExc_AttributeErrorType                m_PyExc_AttributeError;
    PyExc_ImportErrorType                   m_PyExc_ImportError;
    PyExc_IndexErrorType                    m_PyExc_IndexError;
};

extern ref<PythonLibrary> s_library;

}}

/**************************************************************************************************/
#endif
