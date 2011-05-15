/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_LOADERS_TEXTURELOADER_HH_
#define BE_OPENGL_LOADERS_TEXTURELOADER_HH_
/*****************************************************************************/
#include    <system/resource/iresourceloader.script.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

class TextureLoader : public IResourceLoader
{
public:
    TextureLoader();
    ~TextureLoader();

    virtual void* load(weak<const Resource> source) const override;
    virtual void  unload(const void* resource) const override;
};

}}}

/*****************************************************************************/
#endif
