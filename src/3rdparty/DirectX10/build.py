
def options(opt):
    opt.recurse('../DirectX/build')

def setup(conf):
    conf.recurse('../DirectX/build', name='setup')

def build(bld):
    bld.recurse('../DirectX/build')
