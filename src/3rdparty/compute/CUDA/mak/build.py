def build(bld):
    cuda = bld.thirdparty('3rdparty.compute.CUDA')
    if cuda:
        cuda.export_lib += ['cudart_static']

