/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <clkernelscheduler.hh>
#include    <clkernelloader.hh>
#include    <system/resource/resourcemanager.hh>
#include    <system/scheduler/scheduler.hh>
#include    <system/scheduler/kernel/kernel.script.hh>

namespace BugEngine
{

static cl_context createCLContext()
{
    cl_int err;

    cl_platform_id platform;

    cl_uint platformCount = 0;
    err = clGetPlatformIDs(0, 0, &platformCount);
    cl_platform_id* platforms = (cl_platform_id*)malloca(sizeof(cl_platform_id)*platformCount);
    err = clGetPlatformIDs(platformCount, platforms, &platformCount);
    for (cl_uint i = 0; i < platformCount; ++i)
    {
        size_t size = 0;

        clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, 0, 0, &size);
        char* platformVersion = (char*) malloca(size+1);
        platformVersion[size] = 0;
        clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, size, platformVersion, 0);

        clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 0, 0, &size);
        char* platformName = (char*) malloca(size+1);
        platformName[size] = 0;
        clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, size, platformName, 0);

        clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, 0, 0, &size);
        char* vendorName = (char*) malloca(size+1);
        vendorName[size] = 0;
        clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, size, vendorName, 0);

        be_info("Found OpenCL platform %s (%s/%s)" | platformVersion | platformName | vendorName);
        freea(vendorName);
        freea(platformName);
        freea(platformVersion);
    }
    platform = platforms[0];
    freea(platforms);

    cl_device_id device;
    cl_uint deviceCount = 0;
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, 0, &deviceCount);
    cl_device_id* devices = (cl_device_id*)malloca(sizeof(cl_device_id) * deviceCount);
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, deviceCount, devices, 0);

    for (cl_uint i = 0; i < deviceCount; ++i)
    {
        size_t size = 0;

        err = clGetDeviceInfo(devices[i], CL_DEVICE_NAME, 0, 0, &size);
        char* deviceName = (char*)malloca(size+1);
        deviceName[size] = 0;
        err = clGetDeviceInfo(devices[i], CL_DEVICE_NAME, size, deviceName, 0);

        err = clGetDeviceInfo(devices[i], CL_DEVICE_VENDOR, 0, 0, &size);
        char* vendorName = (char*)malloca(size+1);
        vendorName[size] = 0;
        err = clGetDeviceInfo(devices[i], CL_DEVICE_VENDOR, size, vendorName, 0);

        err = clGetDeviceInfo(devices[i], CL_DRIVER_VERSION, 0, 0, &size);
        char* driverVersion = (char*)malloca(size+1);
        driverVersion[size] = 0;
        err = clGetDeviceInfo(devices[i], CL_DRIVER_VERSION, size, driverVersion, 0);

        err = clGetDeviceInfo(devices[i], CL_DEVICE_VERSION, 0, 0, &size);
        char* deviceVersion = (char*)malloca(size+1);
        deviceVersion[size] = 0;
        err = clGetDeviceInfo(devices[i], CL_DEVICE_VERSION, size, deviceVersion, 0);

        err = clGetDeviceInfo(devices[i], CL_DEVICE_PROFILE, 0, 0, &size);
        char* deviceProfile = (char*)malloca(size+1);
        deviceProfile[size] = 0;
        err = clGetDeviceInfo(devices[i], CL_DEVICE_PROFILE, size, deviceProfile, 0);

        be_info("Found %s %s on %s (%s/%s)" | deviceVersion | deviceProfile | deviceName | vendorName | driverVersion);

        freea(deviceProfile);
        freea(deviceVersion);
        freea(driverVersion);
        freea(vendorName);
        freea(deviceName);

        err = clGetDeviceInfo(devices[i], CL_DEVICE_EXTENSIONS, 0, 0, &size);
        char* deviceExtensions = (char*)malloca(size+1);
        deviceExtensions[size] = 0;
        err = clGetDeviceInfo(devices[i], CL_DEVICE_EXTENSIONS, size, deviceExtensions, 0);
        be_info("Extensions: %s" | deviceExtensions);
        freea(deviceExtensions);

    }
    device = devices[0];
    freea(devices);

    cl_context context = clCreateContext(0, 1, &device, 0, 0, &err);
    return context;
}

OpenCLKernelScheduler::OpenCLKernelScheduler(const PluginContext& context)
    :   IKernelScheduler("OpenCL", context.scheduler)
    ,   m_resourceManager(context.resourceManager)
    ,   m_loader(scoped<OpenCLKernelLoader>::create(Arena::task()))
    ,   m_context(createCLContext())
{
    if (m_context)
    {
        m_resourceManager->attach<Kernel>(m_loader);
    }
}

OpenCLKernelScheduler::~OpenCLKernelScheduler()
{
    if (m_context)
    {
        m_resourceManager->detach<Kernel>(m_loader);
        clReleaseContext(m_context);
    }
}

}
