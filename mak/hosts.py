import platform

os_name = platform.uname()[0].lower().split('-')[0]


def options(opt):
    opt.recurse('host/%s' % os_name)


def configure(conf):
    conf.env.HOST = os_name
    conf.recurse('host/%s' % os_name)


def build(bld):
    bld.recurse('host/%s' % os_name)


def plugins(bld):
    bld.recurse('host/%s' % os_name)
