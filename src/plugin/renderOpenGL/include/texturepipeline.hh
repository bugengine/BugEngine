/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_TEXTUREPIPELINE_HH_
#define BE_OPENGL_TEXTUREPIPELINE_HH_
/*****************************************************************************/
#include    <graphics/material/texturepipeline.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

class Renderer;

class TexturePipeline : public Graphics::TexturePipeline
{
private:
    weak<Renderer>   m_owner;
public:
    TexturePipeline(weak<Renderer> owner);
    ~TexturePipeline();

    _Texture*               load(const ifilename& file) override;
    void                    unload(_Texture* s) override;
};

}}}

/*****************************************************************************/
#endif
