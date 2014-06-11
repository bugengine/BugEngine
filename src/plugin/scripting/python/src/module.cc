/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonlib.hh>


static PyMethodDef s_methods[] =
{
    {NULL, NULL, 0, NULL}
};

extern "C" BE_EXPORT void initpy_bugengine()
{
    be_info("loading module py_bugengine");
    static ref<BugEngine::Python::PythonLibrary> library =
                ref<BugEngine::Python::PythonLibrary>::create(BugEngine::Arena::general(), (const char*)NULL);
    (*library->m_Py_InitModule3)("py_bugengine", s_methods, "");
}

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

extern "C" BE_EXPORT void PyInit_py_bugengine()
{
    be_info("loading module py_bugengine");
    static ref<BugEngine::Python::PythonLibrary> library =
                ref<BugEngine::Python::PythonLibrary>::create(BugEngine::Arena::general(), (const char*)NULL);
    (*library->m_PyModule_Create2)(&s_module, library->getVersion());
}
