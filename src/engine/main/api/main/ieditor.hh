/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MAIN_IEDITOR_HH_
#define BE_MAIN_IEDITOR_HH_
/*****************************************************************************/
#include    <system/filesystem.hh>
#include    <main/ieditor.hh>

namespace BugEngine { namespace Editor
{

class be_api(MAIN) IEditor : public minitl::pointer
{
private:
    ref<FileSystem> m_filesystem;
public:
    IEditor();
    ~IEditor();
};

}}

/*****************************************************************************/
#endif
