/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHONLIB_PY_CLASS_HH_
#define BE_PYTHONLIB_PY_CLASS_HH_
/**************************************************************************************************/
#include    <pythonlib/stdafx.h>
#include    <py_namespace.hh>

namespace BugEngine { namespace Python
{

struct PyBugClass : public PyBugNamespace
{
    static PyObject* create(PyObject* owner, const RTTI::Value& value);

    static void registerType(PyObject* module);
    static PyTypeObject s_pyType;
};

}}

/**************************************************************************************************/
#endif
