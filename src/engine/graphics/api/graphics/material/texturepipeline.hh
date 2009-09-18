/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_MATERIAL_TEXTUREPIPELINE_HH_
#define BE_GRAPHICS_MATERIAL_TEXTUREPIPELINE_HH_
/*****************************************************************************/
#include    <system/resource/pipeline.hh>

namespace BugEngine { namespace Graphics
{

class _Texture;

class TexturePipeline : public System::Pipeline
{
public:
    virtual _Texture*           load(const ifilename& file) = 0;
    virtual void                unload(_Texture* s) = 0;
};

}}

/*****************************************************************************/
#endif
