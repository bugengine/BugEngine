#include    <pythonlib/stdafx.h>
#include    <pythonlib/pythonlib.hh>
#include    <core/environment.hh>

extern "C" BE_EXPORT void initpy_bugengine()
{
    using namespace BugEngine;
    using namespace BugEngine::Python;
    /* python 2.x module initialisation */
    Environment::getEnvironment().init();
    be_info("loading module py_bugengine (Python 2)");
    static ref<PythonLibrary> s_library = ref<PythonLibrary>::create(Arena::general(), (const char*)0);
    setCurrentContext(s_library);
    init2_py_bugengine(false);
}

extern "C" BE_EXPORT BugEngine::Python::PyObject* PyInit_py_bugengine()
{
    using namespace BugEngine;
    using namespace BugEngine::Python;
    /* python 3.x module initialisation */
    Environment::getEnvironment().init();
    static ref<PythonLibrary> s_library = ref<PythonLibrary>::create(Arena::general(), (const char*)0);
    setCurrentContext(s_library);
    be_info("loading module py_bugengine (Python 3)");
    PyObject* module = init3_py_bugengine(false);
    return module;
}
