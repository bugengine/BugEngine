def build(bld):
    cl = bld.thirdparty('3rdparty.compute.OpenCL', feature='OpenCL', use=['3rdparty.graphics.OpenGL'])