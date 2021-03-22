/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_PYTHONLIB_PYTHONSCRIPT_SCRIPT_HH_
#define BE_PYTHONLIB_PYTHONSCRIPT_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/plugin.scripting.pythonlib/stdafx.h>
#include <bugengine/meta/tags/editor.script.hh>
#include <bugengine/script.script.hh>

namespace BugEngine {

class be_api(PYTHON) PythonScript : public Script
{
    friend class PackageLoader;
published:
    PythonScript(be_tag(EditHint::Extension(".py")) weak< const File > script);
    ~PythonScript();
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
