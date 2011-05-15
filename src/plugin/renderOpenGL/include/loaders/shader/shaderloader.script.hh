/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_LOADERS_SHADERLOADER_HH_
#define BE_OPENGL_LOADERS_SHADERLOADER_HH_
/*****************************************************************************/
#include    <system/resource/iresourceloader.script.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

class ShaderLoader : public IResourceLoader
{
public:
    ShaderLoader();
    ~ShaderLoader();
public:
    struct ShaderContext
    {
        GLuint  shader;

        ShaderContext(GLenum shaderType, const char *text, i64 textSize);
        ~ShaderContext();
    };

    virtual void* load(weak<const Resource> source) const override;
    virtual void  unload(const void* resource) const override;
};

}}}

/*****************************************************************************/
#endif
