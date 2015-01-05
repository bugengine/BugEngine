/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHON_PYTHONTYPES_HH_
#define BE_PYTHON_PYTHONTYPES_HH_
/**************************************************************************************************/
#include <cstdio>

namespace BugEngine { namespace Python
{

#ifdef _LP64
typedef i64 Py_ssize_t;
#else
typedef i32 Py_ssize_t;
#endif
typedef Py_ssize_t Py_hash_t;

struct PyThreadState;
struct PyObject;
struct PyTypeObject;
struct PyMethodDef;
struct PyModuleDef;
struct PyMemberDef;
struct PyGetSetDef;


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
typedef int (*PyModule_AddObjectType)(PyObject* module, const char* name, PyObject* value);

typedef void (*PyEval_InitThreadsType)();
typedef PyThreadState* (*PyEval_SaveThreadType)();
typedef void (*PyEval_AcquireThreadType)(PyThreadState* tstate);
typedef void (*PyEval_ReleaseThreadType)(PyThreadState* tstate);
typedef void (*PyEval_ReleaseLockType)();

typedef int (*PyRun_SimpleStringType)(const char* command);

typedef PyObject* (*PyCFunction)(PyObject* self, PyObject* args);

typedef PyObject* (*unaryfunc)(PyObject *);
typedef PyObject* (*binaryfunc)(PyObject *, PyObject *);
typedef PyObject* (*ternaryfunc)(PyObject *, PyObject *, PyObject *);
typedef Py_ssize_t (*lenfunc)(PyObject *);
typedef PyObject *(*ssizeargfunc)(PyObject *, Py_ssize_t);
typedef PyObject *(*ssizessizeargfunc)(PyObject *, Py_ssize_t, Py_ssize_t);
typedef int(*ssizeobjargproc)(PyObject *, Py_ssize_t, PyObject *);
typedef int(*ssizessizeobjargproc)(PyObject *, Py_ssize_t, Py_ssize_t, PyObject *);
typedef int(*objobjargproc)(PyObject *, PyObject *, PyObject *);

typedef int (*visitproc)(PyObject *object, void *arg);
typedef int (*traverseproc)(PyObject *self, visitproc visit, void *arg);
typedef int (*inquiry)(PyObject *self);
typedef void (*freefunc)(void *);
typedef void (*destructor)(PyObject *);
typedef int (*printfunc)(PyObject *, FILE*, int);
typedef int (*cmpfunc)(PyObject*, PyObject*);
typedef PyObject* (*reprfunc)(PyObject*);
typedef void (*freefunc)(void*);
typedef PyObject* (*getattrfunc)(PyObject *, const char *);
typedef PyObject* (*getattrofunc)(PyObject *, PyObject *);
typedef int (*setattrfunc)(PyObject *, const char *, PyObject *);
typedef int (*setattrofunc)(PyObject *, PyObject *, PyObject *);
typedef Py_hash_t (*hashfunc)(PyObject *);
typedef PyObject *(*richcmpfunc) (PyObject *, PyObject *, int);
typedef PyObject *(*getiterfunc) (PyObject *);
typedef PyObject *(*iternextfunc) (PyObject *);
typedef PyObject *(*descrgetfunc) (PyObject *, PyObject *, PyObject *);
typedef int (*descrsetfunc) (PyObject *, PyObject *, PyObject *);
typedef int (*initproc)(PyObject *, PyObject *, PyObject *);
typedef PyObject *(*newfunc)(PyTypeObject*, PyObject *, PyObject *);
typedef PyObject *(*allocfunc)(PyTypeObject *, Py_ssize_t);


typedef int (*PyObject_SetAttrStringType)(PyObject* o, const char* name, PyObject* value);
typedef PyObject* (*PyObject_GetAttrStringType)(PyObject* o, const char* name);
typedef int (*PyType_ReadyType)(PyTypeObject* type);

typedef PyObject* (*PyList_NewType)(Py_ssize_t len);
typedef Py_ssize_t (*PyList_SizeType)(PyObject *list);
typedef PyObject* (*PyList_GetItemType)(PyObject *list, Py_ssize_t index);
typedef int (*PyList_SetItemType)(PyObject *list, Py_ssize_t index, PyObject *item);
typedef int (*PyList_InsertType)(PyObject *list, Py_ssize_t index, PyObject *item);
typedef int (*PyList_AppendType)(PyObject *list, PyObject *item);
typedef PyObject* (*PyList_GetSliceType)(PyObject *list, Py_ssize_t low, Py_ssize_t high);
typedef int (*PyList_SetSliceType)(PyObject *list, Py_ssize_t low, Py_ssize_t high, PyObject *itemlist);

typedef PyObject* (*PyString_FromStringType)(const char *v);
typedef PyObject* (*PyString_FromStringAndSizeType)(const char *v, Py_ssize_t len);
typedef PyObject* (*PyString_FromFormatType)(const char *format, ...);
typedef Py_ssize_t (*PyString_SizeType)(PyObject *string);
typedef char* (*PyString_AsStringType)(PyObject *string);
typedef int (*PyString_AsStringAndSizeType)(PyObject *obj, char **buffer, Py_ssize_t *length);

typedef PyObject* (*PyUnicode_FromStringType)(const char *v);
typedef PyObject* (*PyUnicode_FromStringAndSizeType)(const char *v, Py_ssize_t len);
typedef PyObject* (*PyUnicode_FromFormatType)(const char *format, ...);

typedef PyObject* _Py_NoneStructType;

struct PyObject
{
    minitl::size_type   py_refcount;
    PyTypeObject*       py_type;
};

struct PyVarObject
{
    PyObject    object;
    Py_ssize_t  ob_size;
};

struct PyTypeObject
{
    PyVarObject         object;
    const char*         tp_name;
    minitl::size_type   tp_basicsize;
    minitl::size_type   tp_itemsize;

    destructor          tp_dealloc;
    printfunc           tp_print;
    getattrfunc         tp_getattr;
    setattrfunc         tp_setattr;
    cmpfunc             tp_compare;
    reprfunc            tp_repr;

    void*               tp_as_number;
    void*               tp_as_sequence;
    void*               tp_as_mapping;

    hashfunc            tp_hash;
    ternaryfunc         tp_call;
    reprfunc            tp_str;
    getattrofunc        tp_getattro;
    setattrofunc        tp_setattro;

    void*               tp_as_buffer;

    unsigned long       tp_flags;

    const char*         tp_doc;
    traverseproc        tp_traverse;
    inquiry             tp_clear;
    richcmpfunc         tp_richcompare;
    Py_ssize_t          tp_weaklistoffset;
    getiterfunc         tp_iter;
    iternextfunc        tp_iternext;
    PyMethodDef*        tp_methods;
    PyMemberDef*        tp_members;
    PyGetSetDef*        tp_getset;
    PyTypeObject*       tp_base;
    PyObject*           tp_dict;
    descrgetfunc        tp_descr_get;
    descrsetfunc        tp_descr_set;
    Py_ssize_t          tp_dictoffset;
    initproc            tp_init;
    allocfunc           tp_alloc;
    newfunc             tp_new;
    freefunc            tp_free;
    inquiry             tp_is_gc;
    PyObject*           tp_bases;
    PyObject*           tp_mro;
    PyObject*           tp_cache;
    PyObject*           tp_subclasses;
    PyObject*           tp_weaklist;
    destructor          tp_del;

    unsigned int        tp_version_tag;
    destructor          tp_finalize;
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


#define Py_INCREF(pyobject)                                                                         \
    do                                                                                              \
    {                                                                                               \
        BugEngine::Python::PyObject* o = reinterpret_cast<BugEngine::Python::PyObject*>(pyobject);  \
        ++ o->py_refcount;                                                                          \
    } while (0)
#define Py_DECREF(pyobject)                                                                         \
    do                                                                                              \
    {                                                                                               \
        BugEngine::Python::PyObject* o = reinterpret_cast<BugEngine::Python::PyObject*>(pyobject);  \
        if (--o->py_refcount == 0)                                                                  \
        {                                                                                           \
            o->py_type->tp_dealloc(o);                                                              \
        }                                                                                           \
    } while (0)

}}

/**************************************************************************************************/
#endif
