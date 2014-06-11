/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHON_PYTHONTYPES_HH_
#define BE_PYTHON_PYTHONTYPES_HH_
/**************************************************************************************************/

extern "C"
{

struct PyThreadState;
struct PyObject;
struct PyMethodDef;
struct PyModuleDef;


typedef void (*Py_SetPythonHomeFunc)(const char* home);
typedef void (*Py_InitializeExFunc)(int initsigs);
typedef void (*Py_FinalizeFunc)();
typedef PyThreadState* (*Py_NewInterpreterFunc)();
typedef void (*Py_EndInterpreterFunc)(PyThreadState* tstate);
typedef const char* (*Py_GetPathFunc)();
typedef const char* (*Py_GetVersionFunc)();
typedef PyObject* (*Py_InitModule4Func)(const char* name, PyMethodDef* methods,
                                        const char* doc, PyObject* self, int apiver);
typedef PyObject* (*Py_InitModule4_64Func)(const char* name, PyMethodDef* methods,
                                           const char* doc, PyObject* self, int apiver);

typedef PyObject* (*PyModule_Create2Func)(PyModuleDef* module, int apiver);

typedef void (*PyEval_InitThreadsFunc)();
typedef PyThreadState* (*PyEval_SaveThreadFunc)();
typedef void (*PyEval_AcquireThreadFunc)(PyThreadState* tstate);
typedef void (*PyEval_ReleaseThreadFunc)(PyThreadState* tstate);
typedef void (*PyEval_ReleaseLockFunc)();

typedef int (*PyRun_SimpleStringFunc)(const char* command);

typedef PyObject* (*PyCFunction)(PyObject* self, PyObject* args);

typedef int (*visitproc)(PyObject *object, void *arg);
typedef int (*traverseproc)(PyObject *self, visitproc visit, void *arg);
typedef int (*inquiry)(PyObject *self);
typedef void (*freefunc)(void*);

struct PyObject
{
    minitl::size_type   py_refcount;
    PyObject*           py_type;
};

struct PyMethodDef
{
    const char* const   name;
    PyCFunction         method;
    int                 flags;
    const char* const   doc;
};

/* Flag passed to newmethodobject */
#define METH_OLDARGS  0x0000
#define METH_VARARGS  0x0001
#define METH_KEYWORDS 0x0002
/* METH_NOARGS and METH_O must not be combined with the flags above. */
#define METH_NOARGS   0x0004
#define METH_O        0x0008
/* METH_CLASS and METH_STATIC are a little different; these control
   the construction of methods for a class.  These cannot be used for
   functions in modules. */
#define METH_CLASS    0x0010
#define METH_STATIC   0x0020
/* METH_COEXIST allows a method to be entered eventhough a slot has
   already filled the entry.  When defined, the flag allows a separate
   method, "__contains__" for example, to coexist with a defined
   slot like sq_contains. */
#define METH_COEXIST  0x0040


struct PyModuleDef_Base
{
    PyObject ob_base;
    PyObject* (*m_init)(void);
    minitl::size_type m_index;
    PyObject* m_copy;
};

#define PyModuleDef_HEAD_INIT { \
    { 1, NULL },/* ob_base */   \
    NULL,       /* m_init */    \
    0,          /* m_index */   \
    NULL,       /* m_copy */    \
  }

struct PyModuleDef
{
    PyModuleDef_Base m_base;
    const char* m_name;
    const char* m_doc;
    minitl::size_type m_size;
    PyMethodDef *m_methods;
    inquiry m_reload;
    traverseproc m_traverse;
    inquiry m_clear;
    freefunc m_free;
};


}


/**************************************************************************************************/
#endif
