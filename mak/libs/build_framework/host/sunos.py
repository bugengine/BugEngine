import os


def options(opt):
    pass


def configure(conf):
    try:
        for extra in os.listdir('/opt'):
            conf.env.append_unique('EXTRA_PATH', os.path.join('/opt', extra, 'bin'))
    except OSError:
        pass
    try:
        for gcc_version in os.listdir('/usr/gcc/'):
            conf.env.append_unique('EXTRA_PATH', os.path.join('/usr', 'gcc', gcc_version, 'bin'))
    except OSError:
        pass


def build(bld):
    pass


def plugins(bld):
    pass
