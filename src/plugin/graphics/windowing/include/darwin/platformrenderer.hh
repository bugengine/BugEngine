/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_WINDOWING_DARWIN_RENDERER_HH_
#define BE_WINDOWING_DARWIN_RENDERER_HH_
/*****************************************************************************/
#include    <windowing/renderer.hh>
#include    <Cocoa/Cocoa.h>

namespace BugEngine { namespace Windowing
{

class Renderer::PlatformRenderer : public minitl::refcountable
{
private:
    NSAutoreleasePool*  m_pool;
    NSApplication*      m_application;
public:
    PlatformRenderer();
    ~PlatformRenderer();
    void flush();
};


}}


/*****************************************************************************/
#endif

