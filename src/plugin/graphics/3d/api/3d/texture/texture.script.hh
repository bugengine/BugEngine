/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_3D_TEXTURE_TEXTURE_SCRIPT_HH_
#define BE_3D_TEXTURE_TEXTURE_SCRIPT_HH_
/**************************************************************************************************/
#include    <3d/stdafx.h>
#include    <resource/description.script.hh>

namespace BugEngine
{

class be_api(3D) TextureDescription : public Resource::Description
{
public:
    TextureDescription();
    ~TextureDescription();
};

}

/**************************************************************************************************/
#endif
