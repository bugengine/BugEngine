/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <opencl/stdafx.h>
#include    <opencl/scheduler.hh>
#include    <opencl/memoryhost.hh>
#include    <codeloader.hh>
#include    <resource/resourcemanager.hh>
#include    <scheduler/scheduler.hh>
#include    <scheduler/task/itask.hh>
#include    <scheduler/kernel/kernel.script.hh>


namespace BugEngine { namespace Kernel { namespace OpenCL
{

#define checkResult(a) do { cl_int err = a; if (err != CL_SUCCESS) be_error("OpenCL call %s failed with error code %d"|#a|err); } while (0)
struct CLStringInfo
{
    enum { InfoLogSize = 1024 };
    char info[InfoLogSize];
};

static CLStringInfo getPlatformInfo(cl_platform_id platform, cl_platform_info name)
{
    CLStringInfo result;
    size_t size = CLStringInfo::InfoLogSize;
    checkResult(clGetPlatformInfo(platform, name, size, result.info, 0));
    return result;
}

static CLStringInfo getDeviceInfo(cl_device_id device, cl_device_info name)
{
    CLStringInfo result;
    size_t size = CLStringInfo::InfoLogSize;
    checkResult(clGetDeviceInfo(device, name, size, result.info, 0));
    return result;
}

cl_context Scheduler::createCLContextOnPlatform(const cl_context_properties* properties,
                                                cl_platform_id platform,
                                                cl_device_type deviceType)
{
    cl_device_id device;
    cl_context context = 0;
    cl_uint deviceCount = 0;
    cl_int error = clGetDeviceIDs(platform, deviceType, 0, 0, &deviceCount);
    if (error == CL_DEVICE_NOT_FOUND)
        return context;
    checkResult(error);
    if (deviceCount > 0)
    {
        cl_device_id* devices = (cl_device_id*)malloca(sizeof(cl_device_id) * deviceCount);
        checkResult(clGetDeviceIDs(platform, deviceType, deviceCount, devices, 0));

        for (cl_uint i = 0; i < deviceCount; ++i)
        {
            device = devices[i];
            be_info("Found %s %s on %s (%s/%s)"
                    |   getDeviceInfo(device, CL_DEVICE_VERSION).info
                    |   getDeviceInfo(device, CL_DEVICE_PROFILE).info
                    |   getDeviceInfo(device, CL_DEVICE_NAME).info
                    |   getDeviceInfo(device, CL_DEVICE_VENDOR).info
                    |   getDeviceInfo(device, CL_DRIVER_VERSION).info);
            size_t size = 0;
            checkResult(clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, 0, 0, &size));
            char* deviceExtensions = (char*)malloca(size+1);
            deviceExtensions[size] = 0;
            checkResult(clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, size, deviceExtensions, 0));
            be_info("Extensions: %s" | deviceExtensions);
            freea(deviceExtensions);

            cl_int err;
            context = clCreateContext(properties, 1, &device, 0, 0, &err);
            if (err == CL_SUCCESS)
                break;
        }
        freea(devices);
    }

    return context;
}

cl_context Scheduler::createCLContext(const cl_context_properties* properties)
{
    cl_context context = 0;

    cl_uint platformCount = 0;
    checkResult(clGetPlatformIDs(0, 0, &platformCount));
    cl_platform_id* platforms = (cl_platform_id*)malloca(sizeof(cl_platform_id)*platformCount);
    checkResult(clGetPlatformIDs(platformCount, platforms, &platformCount));
    for (cl_uint i = 0; i < platformCount; ++i)
    {
        cl_platform_id p = platforms[i];
        be_info("Found OpenCL platform %s (%s/%s)"
                |   getPlatformInfo(p, CL_PLATFORM_NAME).info
                |   getPlatformInfo(p, CL_PLATFORM_VENDOR).info
                |   getPlatformInfo(p, CL_PLATFORM_VERSION).info);
        context = createCLContextOnPlatform(properties, p, CL_DEVICE_TYPE_ACCELERATOR);
        if (!context)
        {
            context = createCLContextOnPlatform(properties, p, CL_DEVICE_TYPE_GPU);
        }
        if (!context)
        {
            context = createCLContextOnPlatform(properties, p, CL_DEVICE_TYPE_CPU);
        }
        if (!context)
        {
            context = createCLContextOnPlatform(properties, p, CL_DEVICE_TYPE_ALL);
        }
        if (context)
            break;
    }
    freea(platforms);
    return context;
}

Scheduler::Scheduler(const Plugin::Context& context,
                     const cl_context_properties* properties)
    :   IScheduler("OpenCL", context.scheduler)
    ,   m_resourceManager(context.resourceManager)
    ,   m_loader(scoped<CodeLoader>::create(Arena::task()))
    ,   m_memoryHost(scoped<MemoryHost>::create(Arena::task()))
    ,   m_context(createCLContext(properties))
{
    if (m_context)
    {
        m_resourceManager->attach<Kernel>(weak<Resource::ILoader>(m_loader));
    }
}

Scheduler::~Scheduler()
{
    if (m_context)
    {
        m_resourceManager->detach<Kernel>(weak<const Resource::ILoader>(m_loader));
        clReleaseContext(m_context);
    }
}

void Scheduler::run(weak<Task::KernelTask> task,
                    weak<const Kernel> kernel,
                    const minitl::array< weak<const IMemoryBuffer> >& parameters)
{
    be_forceuse(kernel);
    be_forceuse(parameters);
    be_notreached();
    task->completed(m_scheduler);
}

weak<IMemoryHost> Scheduler::memoryHost() const
{
    return m_memoryHost;
}

}}}