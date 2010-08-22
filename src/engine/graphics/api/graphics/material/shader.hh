/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_MATERIAL_SHADER_HH_
#define BE_GRAPHICS_MATERIAL_SHADER_HH_
/*****************************************************************************/
#include    <system/resource/resource.hh>

namespace BugEngine { namespace Graphics
{

class be_api(GRAPHICS) Shader : Resource
{
private:
public:
    Shader();
    ~Shader();
};

}}

/*****************************************************************************/
#endif
