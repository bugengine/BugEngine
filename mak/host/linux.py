import os


def add_ld_so(conf, filename, paths):
    f = open(filename, 'r')
    for line in f:
        line = line.strip()
        if not line:
            continue
        elif line.startswith('#'):
            continue
        elif line.startswith('include'):
            continue
        elif os.path.isdir(line):
            paths.append(line)


def options(opt):
    pass


def configure(conf):
    lib_paths = []
    if os.path.isfile('/etc/ld.so.conf'):
        add_ld_so(conf, '/etc/ld.so.conf', lib_paths)
    if os.path.isdir('/etc/ld.so.conf.d'):
        for f in sorted(os.listdir('/etc/ld.so.conf.d')):
            add_ld_so(conf, '/etc/ld.so.conf.d/'+f, lib_paths)
    conf.env.SYSTEM_LIB_PATHS = ['/lib', '/usr/lib'] + lib_paths


def build(bld):
    pass


def plugins(bld):
    pass

