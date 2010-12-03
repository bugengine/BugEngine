/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_MATERIAL_ISHADERBUILDER_HH_
#define BE_GRAPHICS_MATERIAL_ISHADERBUILDER_HH_
/*****************************************************************************/
#include    <core/memory/streams.hh>


namespace BugEngine { namespace Graphics
{

class be_api(GRAPHICS) IShaderBuilder
{
private:
    MemoryStream m_stream;
public:
    IShaderBuilder();
    ~IShaderBuilder();
};

}}

/*****************************************************************************/
#endif
