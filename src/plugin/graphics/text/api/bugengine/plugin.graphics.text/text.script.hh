/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_TEXT_TEXT_SCRIPT_HH_
#define BE_TEXT_TEXT_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.text/stdafx.h>
#include <bugengine/resource/description.script.hh>

namespace BugEngine {

class be_api(TEXT) Text : public Resource::Description
{
public:
    Text();
    ~Text();
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
