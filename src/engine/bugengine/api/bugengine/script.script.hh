/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_BUGENGINE_SCRIPT_SCRIPT_HH_
#define BE_BUGENGINE_SCRIPT_SCRIPT_HH_
/*****************************************************************************/
#include    <filesystem/file.script.hh>
#include    <resource/resource.script.hh>

namespace BugEngine
{

template< typename T >
class ScriptEngine;
class BE_EXPORT Script : public Resource
{
    template< typename T >
    friend class ScriptEngine;
private:
    weak<const File>  m_file;
protected:
    Script(weak<const File> file);
    ~Script();
};

}

/*****************************************************************************/
#endif
