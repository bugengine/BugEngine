/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.scripting.pythonlib/stdafx.h>
#include <bugengine/plugin.scripting.pythonlib/pythonlib.hh>
#include <bugengine/plugin/plugin.hh>
#include <py_array.hh>
#include <py_class.hh>
#include <py_enum.hh>
#include <py_log.hh>
#include <py_namespace.hh>
#include <py_number.hh>
#include <py_object.hh>
#include <py_plugin.hh>
#include <py_string.hh>

namespace BugEngine { namespace Python {

tls< PythonLibrary > s_library;
PyObject*            s_moduleObject;

static PyObject* run(PyObject* self, PyObject* args)
{
    be_forceuse(self);
    be_forceuse(args);
    s_library->m_PyRun_InteractiveLoopFlags(stdin, "<interactive>", 0);
    Py_INCREF(s_library->m__Py_NoneStruct);
    return s_library->m__Py_NoneStruct;
}

static PyMethodDef s_methods[] = {{"run", &run, METH_NOARGS, NULL}, {NULL, NULL, 0, NULL}};

static PyModuleDef s_module
    = {PyModuleDef_HEAD_INIT, "py_bugengine", "", 0, s_methods, NULL, NULL, NULL, NULL};

static void setupModule(PyObject* module, bool registerLog)
{
    if(registerLog)
    {
        PyBugLog::registerType(module);
    }
    PyBugPlugin::registerType(module);
    PyBugObject::registerType(module);
    PyBugEnum::registerType(module);
    PyBugNumber< bool >::registerType(module);
    PyBugNumber< u8 >::registerType(module);
    PyBugNumber< u16 >::registerType(module);
    PyBugNumber< u32 >::registerType(module);
    PyBugNumber< u64 >::registerType(module);
    PyBugNumber< i8 >::registerType(module);
    PyBugNumber< i16 >::registerType(module);
    PyBugNumber< i32 >::registerType(module);
    PyBugNumber< i64 >::registerType(module);
    PyBugNumber< float >::registerType(module);
    PyBugNumber< double >::registerType(module);
    PyBugString< istring >::registerType(module);
    PyBugString< inamespace >::registerType(module);
    PyBugString< ipath >::registerType(module);
    PyBugString< ifilename >::registerType(module);
    PyBugArray::registerType(module);
    PyBugNamespace::registerType(module);
    PyBugClass::registerType(module);
    s_moduleObject = module;
}

PyObject* init2_py_bugengine(bool registerLog)
{
    /* python 2.x module initialisation */
    PyObject* module;
    if(s_library->m_Py_InitModule4)
    {
        be_assert(sizeof(minitl::size_type) == 4, "Python is 32bits but BugEngine is 64bits");
        module = (*s_library->m_Py_InitModule4)("py_bugengine", s_methods, "", NULL,
                                                s_library->getApi());
    }
    else if(s_library->m_Py_InitModule4_64)
    {
        be_assert(sizeof(minitl::size_type) == 8, "Python is 64bits but BugEngine is 32bits");
        module = (*s_library->m_Py_InitModule4_64)("py_bugengine", s_methods, "", NULL,
                                                   s_library->getApi());
    }
    else
    {
        be_unimplemented();
        return 0;
    }
    setupModule(module, registerLog);
    return module;
}

PyObject* init3_py_bugengine(bool registerLog)
{
    /* python 3.x module initialisation */
    PyObject* module = (*s_library->m_PyModule_Create2)(&s_module, s_library->getApi());
    setupModule(module, registerLog);
    return module;
}

static void init2_py_bugengine_log()
{
    be_assert(s_library, "Current Python context not set; call setCurrentContext");
    init2_py_bugengine(true);
}

static PyObject* init3_py_bugengine_log()
{
    be_assert(s_library, "Current Python context not set; call setCurrentContext");
    return init3_py_bugengine(true);
}

static void registerModule()
{
    using namespace BugEngine::Python;
    if(s_library->getVersion() < 30)
    {
        s_library->m_PyImport_AppendInittab2("py_bugengine", &init2_py_bugengine_log);
    }
    else
    {
        s_library->m_PyImport_AppendInittab3("py_bugengine", &init3_py_bugengine_log);
    }
}

ref< PythonLibrary > loadPython(const char* pythonPath)
{
    ref< PythonLibrary > library = ref< PythonLibrary >::create(Arena::general(), pythonPath);
    if(library)
    {
        setCurrentContext(library);
        registerModule();
        library->initialize();
        clearCurrentContext();
    }
    return library;
}

void setCurrentContext(weak< PythonLibrary > library)
{
    s_library = library;
}

void clearCurrentContext()
{
    s_library = 0;
}

}}  // namespace BugEngine::Python
