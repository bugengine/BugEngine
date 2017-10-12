def build(bld):
    bld.thirdparty('3rdparty.compute.OpenCL', feature='OpenCL', use=['3rdparty.graphics.OpenGL'])

