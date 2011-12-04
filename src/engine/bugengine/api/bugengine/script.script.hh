/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_BUGENGINE_SCRIPT_SCRIPT_HH_
#define BE_BUGENGINE_SCRIPT_SCRIPT_HH_
/*****************************************************************************/
#include    <system/file/file.script.hh>
#include    <system/resource/resource.script.hh>

namespace BugEngine
{

template< typename T >
class ScriptEngine;
class BE_EXPORT Script : public Resource
{
    template< typename T >
    friend class ScriptEngine;
private:
    ref<const File>   m_file;
protected:
    Script(ref<const File> file);
    ~Script();
};

}

/*****************************************************************************/
#endif
