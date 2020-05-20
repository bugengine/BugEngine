import os


def build(bld):
    cl = bld.thirdparty('3rdparty.compute.OpenCL', feature='OpenCL', use=['3rdparty.graphics.OpenGL'])
    if bld.env.PROJECTS:
        cl.export_includes = [os.path.join(bld.path.parent.abspath(), 'api')]