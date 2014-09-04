def options(opt):
    opt.recurse('../DirectX/DirectX')

def setup(conf):
    conf.recurse('../DirectX/DirectX', name='setup')
