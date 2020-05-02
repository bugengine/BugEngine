/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.compute.opencl/stdafx.h>
#include <bugengine/plugin.compute.opencl/memoryhost.hh>
#include <bugengine/plugin.compute.opencl/scheduler.hh>
#include <bugengine/resource/resourcemanager.hh>
#include <bugengine/scheduler/kernel/kernel.script.hh>
#include <bugengine/scheduler/scheduler.hh>
#include <bugengine/scheduler/task/itask.hh>
#include <codeloader.hh>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL {

static const int s_profilingMode =
#if BE_OPTIM_LEVEL_AT_MOST(BE_OPTIM_LVEL_PROFILE)
   CL_QUEUE_PROFILING_ENABLE
#else
   0
#endif
   ;

#define checkResult(a)                                                                             \
    do                                                                                             \
    {                                                                                              \
        cl_int err = a;                                                                            \
        if(err != CL_SUCCESS) be_error("OpenCL call %s failed with error code %d" | #a | err);     \
    } while(0)
struct CLStringInfo
{
    enum
    {
        InfoLogSize = 1024
    };
    char info[InfoLogSize];
};

static CLStringInfo getPlatformInfo(cl_platform_id platform, cl_platform_info name)
{
    CLStringInfo result;
    size_t       size = CLStringInfo::InfoLogSize;
    checkResult(clGetPlatformInfo(platform, name, size, result.info, 0));
    return result;
}

static CLStringInfo getDeviceInfo(cl_device_id device, cl_device_info name)
{
    CLStringInfo result;
    size_t       size = CLStringInfo::InfoLogSize;
    checkResult(clGetDeviceInfo(device, name, size, result.info, 0));
    return result;
}

u32 Scheduler::getContextPointerSize(ClContext context)
{
    u32 result;
    checkResult(
       clGetDeviceInfo(context.second, CL_DEVICE_ADDRESS_BITS, sizeof(result), &result, 0));
    return result;
}

Scheduler::ClContext Scheduler::createCLContextOnPlatform(const cl_context_properties* properties,
                                                          cl_platform_id               platform,
                                                          cl_device_type               deviceType)
{
    cl_device_id device      = 0;
    cl_context   context     = 0;
    cl_uint      deviceCount = 0;
    cl_int       error       = clGetDeviceIDs(platform, deviceType, 0, 0, &deviceCount);
    if(error == CL_DEVICE_NOT_FOUND) return minitl::make_tuple(context, device);
    checkResult(error);
    if(deviceCount > 0)
    {
        cl_device_id* devices = (cl_device_id*)malloca(sizeof(cl_device_id) * deviceCount);
        checkResult(clGetDeviceIDs(platform, deviceType, deviceCount, devices, 0));

        for(cl_uint i = 0; i < deviceCount; ++i)
        {
            device = devices[i];
            be_info("Found %s %s on %s (%s/%s)" | getDeviceInfo(device, CL_DEVICE_VERSION).info
                    | getDeviceInfo(device, CL_DEVICE_PROFILE).info
                    | getDeviceInfo(device, CL_DEVICE_NAME).info
                    | getDeviceInfo(device, CL_DEVICE_VENDOR).info
                    | getDeviceInfo(device, CL_DRIVER_VERSION).info);
            size_t size = 0;
            checkResult(clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, 0, 0, &size));
            char* deviceExtensions = (char*)malloca(size + 1);
            deviceExtensions[size] = 0;
            checkResult(clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, size, deviceExtensions, 0));
            be_info("Extensions: %s" | deviceExtensions);
            freea(deviceExtensions);

            cl_int err;
            context = clCreateContext(properties, 1, &device, 0, 0, &err);
            if(err == CL_SUCCESS) break;
        }
        freea(devices);
    }

    return minitl::make_tuple(context, device);
}

Scheduler::ClContext Scheduler::createCLContext(const cl_context_properties* properties)
{
    ClContext context;

    cl_uint platformCount = 0;
    checkResult(clGetPlatformIDs(0, 0, &platformCount));
    cl_platform_id* platforms = (cl_platform_id*)malloca(sizeof(cl_platform_id) * platformCount);
    checkResult(clGetPlatformIDs(platformCount, platforms, &platformCount));
    for(cl_uint i = 0; i < platformCount; ++i)
    {
        cl_platform_id p = platforms[i];
        be_info("Found OpenCL platform %s (%s/%s)" | getPlatformInfo(p, CL_PLATFORM_NAME).info
                | getPlatformInfo(p, CL_PLATFORM_VENDOR).info
                | getPlatformInfo(p, CL_PLATFORM_VERSION).info);
        context = createCLContextOnPlatform(properties, p, CL_DEVICE_TYPE_ACCELERATOR);
        if(!context.first)
        {
            context = createCLContextOnPlatform(properties, p, CL_DEVICE_TYPE_GPU);
        }
        if(!context.first)
        {
            context = createCLContextOnPlatform(properties, p, CL_DEVICE_TYPE_CPU);
        }
        if(!context.first)
        {
            context = createCLContextOnPlatform(properties, p, CL_DEVICE_TYPE_ALL);
        }
        if(context.first) break;
    }
    freea(platforms);
    return context;
}

Scheduler::ClKernel Scheduler::buildKernel(const size_t size, const char* code) const
{
    cl_int     error_code = 0;
    cl_kernel  kernel     = 0;
    cl_program program    = clCreateProgramWithSource(
       m_context.first, 1, reinterpret_cast< const char** >(&code), &size, &error_code);
    if(error_code != CL_SUCCESS)
    {
        be_error("failed to load OpenCL kernel: clCreateProgramWithBinary failed with code %d"
                 | error_code);
        return minitl::make_tuple(kernel, program);
    }

    error_code = clBuildProgram(program, 1, &m_context.second, "", 0, 0);
    cl_program_build_info  info;
    cl_program_binary_type type;
    size_t                 len = 0;
    clGetProgramBuildInfo(program, m_context.second, CL_PROGRAM_BUILD_STATUS, sizeof(info), &info,
                          &len);
    clGetProgramBuildInfo(program, m_context.second, CL_PROGRAM_BINARY_TYPE, sizeof(type), &type,
                          &len);
    clGetProgramBuildInfo(program, m_context.second, CL_PROGRAM_BUILD_LOG, 0, 0, &len);
    if(len > 1)
    {
        char* buffer = (char*)malloca(len);
        clGetProgramBuildInfo(program, m_context.second, CL_PROGRAM_BUILD_LOG, len, buffer, &len);
        be_info("compilation result:\n%s" | buffer);
        freea(buffer);
    }
    if(error_code != CL_SUCCESS)
    {
        be_error("failed to load OpenCL kernel: clBuildProgram failed with code %d" | error_code);
        return minitl::make_tuple(kernel, program);
    }
    checkResult(clGetProgramInfo(program, CL_PROGRAM_BINARIES, 0, 0, &len));
    kernel = clCreateKernel(program, "_kmain", &error_code);
    if(error_code != CL_SUCCESS)
    {
        be_error("failed to load OpenCL kernel: clCreateKernel failed with code %d" | error_code);
        return minitl::make_tuple(kernel, program);
    }
    else
    {
        be_info("success");
    }
    return minitl::make_tuple(kernel, program);
}

Scheduler::Scheduler(const Plugin::Context& context, const cl_context_properties* properties)
    : IScheduler("OpenCL", context.scheduler, GPUType)
    , m_resourceManager(context.resourceManager)
    , m_loader(scoped< CodeLoader >::create(Arena::task(), this))
    , m_memoryHost(scoped< MemoryHost >::create(Arena::task()))
    , m_context(createCLContext(properties))
    , m_errorCode(0)
    , m_commandQueue(
         clCreateCommandQueue(m_context.first, m_context.second, s_profilingMode, &m_errorCode))
    , m_ptrSize(getContextPointerSize(m_context))
{
    if(m_context.first)
    {
        m_resourceManager->attach< Kernel >(weak< Resource::ILoader >(m_loader));
    }
}

Scheduler::~Scheduler()
{
    if(m_context.first)
    {
        m_resourceManager->detach< Kernel >(weak< const Resource::ILoader >(m_loader));
        clReleaseCommandQueue(m_commandQueue);
        clReleaseContext(m_context.first);
    }
}

void Scheduler::run(weak< Task::KernelTask > task, weak< const Kernel > kernel,
                    const minitl::array< weak< const IMemoryBuffer > >& parameters)
{
    be_forceuse(kernel);
    be_forceuse(parameters);
    // be_notreached();
    task->completed(m_scheduler);
}

weak< IMemoryHost > Scheduler::memoryHost() const
{
    return m_memoryHost;
}

}}}  // namespace BugEngine::KernelScheduler::OpenCL
