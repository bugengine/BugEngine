/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RENDERNULL_LOADERS_SHADERLOADER_HH_
#define BE_RENDERNULL_LOADERS_SHADERLOADER_HH_
/*****************************************************************************/
#include    <system/resource/iresourceloader.script.hh>

namespace BugEngine { namespace Graphics { namespace NullRenderer
{

class ShaderLoader : public IResourceLoader
{
public:
    ShaderLoader();
    ~ShaderLoader();

    virtual void* load(weak<const Resource> source) const override;
    virtual void  unload(const void* resource) const override;
};

}}}

/*****************************************************************************/
#endif
