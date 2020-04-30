/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHONLIB_PY_CALL_HH_
#define BE_PYTHONLIB_PY_CALL_HH_
/**************************************************************************************************/
#include <bugengine/plugin.scripting.pythonlib/stdafx.h>
#include <bugengine/plugin.scripting.pythonlib/pythontypes.hh>
#include <py_object.hh>

namespace BugEngine { namespace Python {

PyObject* call(raw< const RTTI::Method > method, PyObject* self, PyObject* args, PyObject* kwargs);

}}  // namespace BugEngine::Python

/**************************************************************************************************/
#endif
