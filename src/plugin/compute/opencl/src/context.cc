/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.compute.opencl/stdafx.h>
#include <context.hh>
#include <platform.hh>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL {

static CLStringInfo getDeviceInfo(cl_device_id device, cl_device_info name)
{
    CLStringInfo result;
    size_t       size = CLStringInfo::InfoLogSize;
    checkResult(clGetDeviceInfo(device, name, size, result.info, 0));
    return result;
}

static u32 getDevicePointerSize(cl_device_id device)
{
    u32 result;
    checkResult(clGetDeviceInfo(device, CL_DEVICE_ADDRESS_BITS, sizeof(result), &result, 0));
    return result;
}

Context::Context(weak< Platform > platform, cl_device_id device, cl_context context)
    : m_platform(platform)
    , m_device(device)
    , m_context(context)
    , m_pointerSize(getDevicePointerSize(device))
{
    be_info("Found %s %s on %s (%s/%s)" | getDeviceInfo(m_device, CL_DEVICE_VERSION).info
            | getDeviceInfo(m_device, CL_DEVICE_PROFILE).info
            | getDeviceInfo(m_device, CL_DEVICE_NAME).info
            | getDeviceInfo(m_device, CL_DEVICE_VENDOR).info
            | getDeviceInfo(m_device, CL_DRIVER_VERSION).info);
    size_t size = 0;
    checkResult(clGetDeviceInfo(m_device, CL_DEVICE_EXTENSIONS, 0, 0, &size));

    char* deviceExtensions         = (char*)malloca(size + 1);
    char* deviceExtensionsIterator = deviceExtensions;
    deviceExtensions[size]         = 0;
    checkResult(clGetDeviceInfo(m_device, CL_DEVICE_EXTENSIONS, size, deviceExtensions, 0));
    while(size > 100)
    {
        char* nextLine = deviceExtensionsIterator + 100;
        size -= 100;
        while(*nextLine != ' ')
        {
            nextLine--;
            size++;
        }
        *nextLine = 0;
        be_info("Extensions: %s" | deviceExtensionsIterator);
        deviceExtensionsIterator = nextLine + 1;
    }
    if(*deviceExtensionsIterator) be_info("Extensions: %s" | deviceExtensionsIterator);
    freea(deviceExtensions);
}

Context::~Context()
{
    clReleaseContext(m_context);
}

cl_program Context::buildProgram(const u64 size, const char* code) const
{
    size_t     codeSize  = be_checked_numcast< size_t >(size);
    cl_int     errorCode = 0;
    cl_program program   = clCreateProgramWithSource(m_context, 1, &code, &codeSize, &errorCode);
    if(errorCode != CL_SUCCESS)
    {
        be_error("failed to load OpenCL kernel: clCreateProgramWithBinary failed with code %d"
                 | errorCode);
        return program;
    }

    errorCode = clBuildProgram(program, 1, &m_device, "", 0, 0);
    cl_program_build_info info;
    size_t                len = 0;
    clGetProgramBuildInfo(program, m_device, CL_PROGRAM_BUILD_STATUS, sizeof(info), &info, &len);
    clGetProgramBuildInfo(program, m_device, CL_PROGRAM_BUILD_LOG, 0, 0, &len);
    if(len > 2)
    {
        minitl::Allocator::Block< char > buffer(Arena::temporary(), len + 1);
        clGetProgramBuildInfo(program, m_device, CL_PROGRAM_BUILD_LOG, (len + 1), buffer.data(),
                              &len);
        be_info("compilation result:\n%s" | buffer.data());
    }
    if(errorCode != CL_SUCCESS)
    {
        be_error("failed to load OpenCL kernel: clBuildProgram failed with code %d" | errorCode);
        return program;
    }
#if CL_VERSION_1_2
    {
        checkResult(clGetProgramInfo(program, CL_PROGRAM_KERNEL_NAMES, 0, 0, &len));
        minitl::Allocator::Block< char > buffer(Arena::temporary(), len + 1);
        clGetProgramInfo(program, CL_PROGRAM_KERNEL_NAMES, (len + 1), buffer.data(), &len);
        be_info("list of kernels: %s" | buffer.data());
        freea(buffer);
    }
#endif
    /*kernel = clCreateKernel(program, "_kmain", &errorCode);
    if(errorCode != CL_SUCCESS)
    {
        be_error("failed to load OpenCL kernel: clCreateKernel failed with code %d" | errorCode);
        return minitl::make_tuple(kernel, program);
    }
    else
    {
        be_info("success");
    }*/
    return program;
}

}}}  // namespace BugEngine::KernelScheduler::OpenCL
