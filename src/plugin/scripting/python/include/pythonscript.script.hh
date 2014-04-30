/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHON27_PYTHONSCRIPT_SCRIPT_HH_
#define BE_PYTHON27_PYTHONSCRIPT_SCRIPT_HH_
/**************************************************************************************************/
#include    <stdafx.h>
#include    <bugengine/script.script.hh>
#include    <rtti/tags/editor.script.hh>

namespace BugEngine
{

class PythonScript : public Script
{
    friend class PackageLoader;
published:
    PythonScript(be_tag(EditHint::Extension(".py")) weak<const File> script);
    ~PythonScript();
};

}

/**************************************************************************************************/
#endif
