/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <kernelobject.hh>
#include    <system/plugin.hh>


namespace BugEngine
{

static void* loadKernel(const inamespace& kernel)
{
    minitl::format<> kernelFile = minitl::format<>(PLUGIN_PREFIX "%s" PLUGIN_EXT) | kernel;
    const ipath& kernelDirectory = Environment::getEnvironment().getDataDirectory();
    static const ipath kernelSubDirectory = ipath("kernels");
    minitl::format<ifilename::MaxFilenameLength> kernelPath = (kernelDirectory + kernelSubDirectory + ifilename(kernelFile.c_str())).str();
    be_info("loading kernel %s (%s)" | kernel | kernelPath);
    void* handle = dlopen(kernelPath.c_str(), RTLD_NOW|RTLD_LOCAL);
    if (!handle)
    {
        be_error(dlerror());
    }
    return handle;
}

KernelObject::KernelObject(const inamespace& name)
    :   m_handle(loadKernel(name))
{
}

KernelObject::~KernelObject()
{
    dlclose(m_handle);
}

}
