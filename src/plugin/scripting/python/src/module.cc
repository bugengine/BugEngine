/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonlib.hh>


static ref<BugEngine::Python::PythonLibrary> s_library;

static PyObject* bugengineGet(PyObject* self, PyObject* args)
{
    be_forceuse(self);
    be_forceuse(args);
    Py_INCREF(s_library->m__Py_NoneStruct);
    return s_library->m__Py_NoneStruct;
}

static PyMethodDef s_methods[] =
{
    {"__getattr__", bugengineGet, METH_OLDARGS, ""},
    {NULL, NULL, 0, NULL}
};

static PyModuleDef s_module =
{
    PyModuleDef_HEAD_INIT,
    "py_bugengine",
    "",
    0,
    s_methods,
    NULL,
    NULL,
    NULL,
    NULL
};

extern "C" BE_EXPORT void initpy_bugengine()
{
    /* python 2.x module initialisation */
    be_info("loading module py_bugengine");
    s_library = ref<BugEngine::Python::PythonLibrary>::create(BugEngine::Arena::general(), (const char*)NULL);
    if (s_library->m_Py_InitModule4)
    {
        be_assert(sizeof(minitl::size_type) == 4, "python size is 4 but bigengine size is %s"
                                                 | sizeof(minitl::size_type));
        (*s_library->m_Py_InitModule4)("py_bugengine", s_methods, "", NULL, s_library->getVersion());
    }
    else if (s_library->m_Py_InitModule4_64)
    {
        be_assert(sizeof(minitl::size_type) == 8, "python size is 8 but bigengine size is %s"
                                                 | sizeof(minitl::size_type));
        (*s_library->m_Py_InitModule4_64)("py_bugengine", s_methods, "", NULL, s_library->getVersion());
    }
    else
    {
        be_unimplemented();
    }
}

extern "C" BE_EXPORT void PyInit_py_bugengine()
{
    /* python 3.x module initialisation */
    be_info("loading module py_bugengine");
    s_library = ref<BugEngine::Python::PythonLibrary>::create(BugEngine::Arena::general(), (const char*)NULL);
    (*s_library->m_PyModule_Create2)(&s_module, s_library->getVersion());
}
