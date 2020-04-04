def build(bld):
    cuda = bld.thirdparty('3rdparty.compute.CUDA', feature='cuda')
    if cuda: cuda.export_lib += ['cudart_static']

