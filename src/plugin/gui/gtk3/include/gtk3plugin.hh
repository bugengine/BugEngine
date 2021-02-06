/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_GUI_GTK3_GTK3PLUGIN_HH_
#define BE_GUI_GTK3_GTK3PLUGIN_HH_
/**************************************************************************************************/
#include <bugengine/core/memory/allocators/system.hh>
#include <glib-object.h>

namespace BugEngine { namespace Gtk3 {

class Gtk3Plugin
{
private:
    struct Page;

private:
    SystemAllocator m_allocator;
    Page*           m_firstPage;
    GQuark          m_bugengineQuark;

public:
    Gtk3Plugin();
    ~Gtk3Plugin();
};

}}  // namespace BugEngine::Gtk3

/**************************************************************************************************/
#endif
