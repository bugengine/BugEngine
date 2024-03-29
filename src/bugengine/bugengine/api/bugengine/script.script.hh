/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_BUGENGINE_SCRIPT_SCRIPT_HH_
#define BE_BUGENGINE_SCRIPT_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/stdafx.h>
#include <bugengine/filesystem/file.script.hh>
#include <bugengine/resource/description.script.hh>

namespace BugEngine {

template < typename T >
class ScriptEngine;
class be_api(BUGENGINE) Script : public Resource::Description
{
    template < typename T >
    friend class ScriptEngine;

private:
    weak< const File > m_file;

protected:
    Script(weak< const File > file);
    ~Script();

public:
    ifilename getScriptName() const;
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
