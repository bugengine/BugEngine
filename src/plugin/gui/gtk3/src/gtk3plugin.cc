/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <stdafx.h>

#include <gtk3plugin.hh>

#include <gobject.hh>
#include <gtk3loader.hh>

#include <bugengine/meta/engine/helper/method.hh>
#include <bugengine/meta/engine/methodinfo.script.hh>
#include <bugengine/meta/engine/namespace.hh>
#include <bugengine/plugin/plugin.hh>

namespace BugEngine { namespace Gtk3 {

struct Gtk3Plugin::Page
{
    Page* next;
    u32   current;
    u8    memory[64 * 1024 - sizeof(Page*) - sizeof(u8*)];
};

Gtk3Plugin::Gtk3Plugin()
    : m_allocator(SystemAllocator::BlockSize_64k, 1)
    , m_firstPage(static_cast< Page* >(m_allocator.allocate()))
    , m_bugengineQuark(g_quark_from_static_string("bugengine"))
{
    gtk_init(0, 0);
    gtk_test_register_all_types();

    m_firstPage->next    = 0;
    m_firstPage->current = 0;
}

Gtk3Plugin::~Gtk3Plugin()
{
}

}}  // namespace BugEngine::Gtk3

BE_PLUGIN_REGISTER(BugEngine::Gtk3::Gtk3Loader);
