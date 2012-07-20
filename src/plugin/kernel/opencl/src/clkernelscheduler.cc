/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <opencl/stdafx.h>
#include    <opencl/clkernelscheduler.hh>
#include    <clkernelloader.hh>
#include    <system/resource/resourcemanager.hh>
#include    <system/scheduler/scheduler.hh>
#include    <system/scheduler/kernel/kernel.script.hh>

namespace BugEngine
{

#define checkResult(a) do { cl_int err = a; if (err != CL_SUCCESS) be_error("OpenCL call %s failed with error code %d"|#a|err); } while (0)

static BugEngine::Debug::Format<1024> getPlatformInfo(cl_platform_id platform, cl_platform_info name)
{
    BugEngine::Debug::Format<1024> result("");
    size_t size = 0;
    checkResult(clGetPlatformInfo(platform, name, 0, 0, &size));
    char* temp = (char*)malloca(size+1);
    checkResult(clGetPlatformInfo(platform, name, size, temp, 0));
    result.append(temp);
    freea(temp);
    return result;
}

static BugEngine::Debug::Format<1024> getDeviceInfo(cl_device_id device, cl_device_info name)
{
    BugEngine::Debug::Format<1024> result("");
    size_t size = 0;
    checkResult(clGetDeviceInfo(device, name, 0, 0, &size));
    char* temp = (char*)malloca(size+1);
    checkResult(clGetDeviceInfo(device, name, size, temp, 0));
    result.append(temp);
    freea(temp);
    return result;
}

cl_context OpenCLKernelScheduler::createCLContextOnPlatform(const cl_context_properties* properties, cl_platform_id platform, cl_device_type deviceType)
{
    cl_device_id device;
    cl_context context = 0;
    cl_uint deviceCount = 0;
    checkResult(clGetDeviceIDs(platform, deviceType, 0, 0, &deviceCount));
    if (deviceCount > 0)
    {
        cl_device_id* devices = (cl_device_id*)malloca(sizeof(cl_device_id) * deviceCount);
        checkResult(clGetDeviceIDs(platform, deviceType, deviceCount, devices, 0));

        for (cl_uint i = 0; i < deviceCount; ++i)
        {
            device = devices[i];
            be_info("Found %s %s on %s (%s/%s)"
                    |   getDeviceInfo(device, CL_DEVICE_VERSION)
                    |   getDeviceInfo(device, CL_DEVICE_PROFILE)
                    |   getDeviceInfo(device, CL_DEVICE_NAME)
                    |   getDeviceInfo(device, CL_DEVICE_VENDOR)
                    |   getDeviceInfo(device, CL_DRIVER_VERSION));
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

cl_context OpenCLKernelScheduler::createCLContext(const cl_context_properties* properties)
{
    cl_context context;

    cl_uint platformCount = 0;
    checkResult(clGetPlatformIDs(0, 0, &platformCount));
    cl_platform_id* platforms = (cl_platform_id*)malloca(sizeof(cl_platform_id)*platformCount);
    checkResult(clGetPlatformIDs(platformCount, platforms, &platformCount));
    for (cl_uint i = 0; i < platformCount; ++i)
    {
        cl_platform_id p = platforms[i];
        be_info("Found OpenCL platform %s (%s/%s)"
                |   getPlatformInfo(p, CL_PLATFORM_NAME)
                |   getPlatformInfo(p, CL_PLATFORM_VENDOR)
                |   getPlatformInfo(p, CL_PLATFORM_VERSION));
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

OpenCLKernelScheduler::OpenCLKernelScheduler(const PluginContext& context, const cl_context_properties* properties)
    :   IKernelScheduler("OpenCL", context.scheduler, context.scheduler->memoryProvider())
    ,   m_resourceManager(context.resourceManager)
    ,   m_loader(scoped<OpenCLKernelLoader>::create(Arena::task()))
    ,   m_context(createCLContext(properties))
{
    if (m_context)
    {
        m_resourceManager->attach<Kernel::Kernel>(m_loader);
    }
}

OpenCLKernelScheduler::~OpenCLKernelScheduler()
{
    if (m_context)
    {
        m_resourceManager->detach<Kernel::Kernel>(m_loader);
        clReleaseContext(m_context);
    }
}

}
