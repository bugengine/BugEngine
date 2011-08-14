/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_3D_TEXTURE_TEXTURE_SCRIPT_HH_
#define BE_3D_TEXTURE_TEXTURE_SCRIPT_HH_
/*****************************************************************************/
#include    <system/resource/resource.script.hh>

namespace BugEngine { namespace Graphics
{

be_tag(ResourceLoaders())
class be_api(_3D) Texture : public Resource
{
public:
    Texture();
    ~Texture();
};

}}

/*****************************************************************************/
#endif
