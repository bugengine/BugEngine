def build(bld):
    if 'cuda' in bld.env.FEATURES:
        cuda = bld.thirdparty('3rdparty.compute.CUDA')
        cuda.export_lib += ['cudart_static']

