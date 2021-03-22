/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_UI_GTK3_GOBJECT_HH_
#define BE_UI_GTK3_GOBJECT_HH_
/**************************************************************************************************/
#include <bugengine/resource/description.script.hh>

namespace BugEngine {

BE_EXPORT raw< Meta::Class > be_plugin_ui_gtk3_Namespace();

}

namespace BugEngine { namespace Gtk3 {

class GObject : public Resource::Description
{
    published : GObject();
    ~GObject();
};

}}  // namespace BugEngine::Gtk3

/**************************************************************************************************/
#endif
