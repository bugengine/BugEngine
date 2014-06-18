/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/pythonlib.hh>


static PyMethodDef s_methods[] =
{
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
    static ref<BugEngine::Python::PythonLibrary> library =
                ref<BugEngine::Python::PythonLibrary>::create(BugEngine::Arena::general(), (const char*)NULL);
    if (library->m_Py_InitModule4)
    {
        be_assert(sizeof(minitl::size_type) == 4, "python size is 4 but bigengine size is %s"
                                                 | sizeof(minitl::size_type));
        (*library->m_Py_InitModule4)("py_bugengine", s_methods, "", NULL, library->getVersion());
    }
    else if (library->m_Py_InitModule4_64)
    {
        be_assert(sizeof(minitl::size_type) == 8, "python size is 8 but bigengine size is %s"
                                                 | sizeof(minitl::size_type));
        (*library->m_Py_InitModule4_64)("py_bugengine", s_methods, "", NULL, library->getVersion());
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
    static ref<BugEngine::Python::PythonLibrary> library =
                ref<BugEngine::Python::PythonLibrary>::create(BugEngine::Arena::general(), (const char*)NULL);
    (*library->m_PyModule_Create2)(&s_module, library->getVersion());
}
