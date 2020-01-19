from waflib import Utils
import os
import sys


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
            add_ld_so(conf, '/etc/ld.so.conf.d/' + f, lib_paths)
    conf.env.ALL_ARCH_LIBPATHS = lib_paths
    try:
        p = Utils.subprocess.Popen(['/usr/bin/cc', '-v'], stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE)
        out, err = p.communicate()
    except BaseException:
        pass
    else:
        if not isinstance(out, str):
            out = out.decode(sys.stdout.encoding)
        if not isinstance(err, str):
            err = err.decode(sys.stderr.encoding)
        for l in out.split('\n') + err.split('\n'):
            if l.startswith('Target: '):
                conf.env.FREEBSD_HOST_TRIPLE = l[8:]


def build(bld):
    pass


def plugins(bld):
    pass
