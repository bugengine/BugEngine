
def options(opt):
    pass

def setup(conf):
    pass

def build(bld):
    bld.static_library('3rdparty.minizip', bld.platforms + ['3rdparty.zlib'], use_master = True, warnings=False)
