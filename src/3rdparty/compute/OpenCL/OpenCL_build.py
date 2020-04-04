def build(bld):
    cl = bld.thirdparty('3rdparty.compute.OpenCL', feature='OpenCL', use=['3rdparty.graphics.OpenGL'])
    if cl: cl.export_defines.append('CL_TARGET_OPENCL_VERSION=120')

