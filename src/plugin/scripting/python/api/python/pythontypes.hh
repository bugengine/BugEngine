/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHON_PYTHONTYPES_HH_
#define BE_PYTHON_PYTHONTYPES_HH_
/**************************************************************************************************/
#include <cstdio>

extern "C"
{

struct PyThreadState;
struct PyObject;
struct PyTypeObject;
struct PyMethodDef;
struct PyModuleDef;


typedef void (*Py_SetPythonHomeType)(const char* home);
typedef void (*Py_InitializeExType)(int initsigs);
typedef void (*Py_FinalizeType)();
typedef PyThreadState* (*Py_NewInterpreterType)();
typedef void (*Py_EndInterpreterType)(PyThreadState* tstate);
typedef const char* (*Py_GetPathType)();
typedef const char* (*Py_GetVersionType)();
typedef PyObject* (*Py_InitModule4Type)(const char* name, PyMethodDef* methods,
                                        const char* doc, PyObject* self, int apiver);
typedef PyObject* (*Py_InitModule4_64Type)(const char* name, PyMethodDef* methods,
                                           const char* doc, PyObject* self, int apiver);

typedef PyObject* (*PyModule_Create2Type)(PyModuleDef* module, int apiver);

typedef void (*PyEval_InitThreadsType)();
typedef PyThreadState* (*PyEval_SaveThreadType)();
typedef void (*PyEval_AcquireThreadType)(PyThreadState* tstate);
typedef void (*PyEval_ReleaseThreadType)(PyThreadState* tstate);
typedef void (*PyEval_ReleaseLockType)();

typedef int (*PyRun_SimpleStringType)(const char* command);

typedef PyObject* (*PyCFunction)(PyObject* self, PyObject* args);

typedef int (*visitproc)(PyObject *object, void *arg);
typedef int (*traverseproc)(PyObject *self, visitproc visit, void *arg);
typedef int (*inquiry)(PyObject *self);
typedef void (*freefunc)(void *);
typedef void (*destructor)(PyObject *);
typedef int (*printfunc)(PyObject *, FILE *, int);
typedef PyObject *(*getattrfunc)(PyObject *, char *);
typedef int (*setattrfunc)(PyObject *, char *, PyObject *);
typedef int (*cmpfunc)(PyObject *, PyObject *);
typedef PyObject *(*reprfunc)(PyObject *);


typedef PyObject* _Py_NoneStructType;

struct PyObject
{
    minitl::size_type   py_refcount;
    PyTypeObject*       py_type;
};

struct PyTypeObject
{
    PyObject            object;
    const char*         tp_name;
    minitl::size_type   tp_basicsize;
    minitl::size_type   tp_itemsize;

    destructor          tp_dealloc;
    printfunc           tp_print;
    getattrfunc         tp_getattr;
    setattrfunc         tp_setattr;
    cmpfunc             tp_compare;
    reprfunc            tp_repr;
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

#define Py_INCREF(pyobject) (++pyobject->py_refcount)
#define Py_DECREF(pyobject) do { if (--pyobject->py_refcount == 0)  } while (0)
}


/**************************************************************************************************/
#endif
