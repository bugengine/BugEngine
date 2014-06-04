
def options(opt):
    opt.recurse('../DirectX/DirectX')

def setup(conf):
    conf.recurse('../DirectX/DirectX', name='setup')

def build(bld):
    bld.recurse('../DirectX/DirectX')
