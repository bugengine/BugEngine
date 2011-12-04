/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_BUGENGINE_IEDITOR_HH_
#define BE_BUGENGINE_IEDITOR_HH_
/*****************************************************************************/
#include    <system/file/folder.script.hh>

namespace BugEngine { namespace Editor
{

class be_api(MAIN) IEditor : public minitl::pointer
{
private:
    ref<Folder> m_folder;
public:
    IEditor();
    ~IEditor();
};

}}

/*****************************************************************************/
#endif
