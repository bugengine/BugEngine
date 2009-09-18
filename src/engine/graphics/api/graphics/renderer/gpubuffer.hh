/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_GPUBUFFER_HH_
#define BE_GRAPHICS_GPUBUFFER_HH_
/*****************************************************************************/

namespace BugEngine { namespace Graphics
{


struct GpuMapFlags
{
};

class GRAPHICSEXPORT GpuBuffer : public minitl::refcountable<void>
{
protected:
    GpuBuffer();
public:
    virtual ~GpuBuffer();

    virtual void* map(GpuMapFlags flags, u32 byteCount, u32 byteOffset) = 0;
    virtual void  unmap() = 0;
};

}}

/*****************************************************************************/
#endif
