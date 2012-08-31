/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_3D_TEXTURE_TEXTURE_SCRIPT_HH_
#define BE_3D_TEXTURE_TEXTURE_SCRIPT_HH_
/*****************************************************************************/
#include    <resource/description.script.hh>

namespace BugEngine
{

class be_api(_3D) TextureDescription : public Resource::Description
{
public:
    TextureDescription();
    ~TextureDescription();
};

}

/*****************************************************************************/
#endif
