from waflib import Utils, Logs
from waflib.Configure import conf
import os, sys

def all_suncc_archs(arch):
    if arch in ['x68', 'i386', 'i486', 'i586', 'i686', 'x64', 'amd64', 'x86_64']:
        return ['x86', 'amd64']
    return [arch]

@conf
def test_suncc(conf, suncc):
    main = conf.bldnode.make_node('main.cpp')
    target = main.change_ext('')
    main.write('#include <stdio.h>\n#include <stdlib.h>\n\nint main() { printf("Hello, world!\\n"); return EXIT_SUCCESS; }\n')
    cmd = [suncc, main.abspath(), '-o', target.abspath(), '-D_GNU_SOURCE']
    success = False
    try:
        p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE)
        out, err = p.communicate()
    except Exception:
        pass
    else:
        success = (p.returncode == 0)
    finally:
        main.delete()
        target.delete()
    return success

@conf
def get_native_suncc_target(conf, suncc):
    cmd = [suncc, '-V']
    try:
        p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE)
        out = p.communicate()[1]
    except:
        return (None, None)

    if not isinstance(out, str):
        out = out.decode(sys.stdout.encoding)
    out = out.split('\n')
    target = os.uname()[0].lower()
    arch = 'x86'
    version = None
    for line in out:
        words = line.split()
        if len(words) > 5:
            if words[0] == 'cc:' and words[1] == 'Sun':
                if words[2] == 'C':
                    version = words[3]
                    target,arch = words[4].split('_')
                else:
                    version = words[4]
                    target,arch = words[5].split('_')
    return [(os.path.split(suncc)[0], target, a, version) for a in all_suncc_archs(arch)]


@conf
def detect_suncc(conf):
    conf.env.SUNCC_TARGETS = []
    bindirs = os.environ['PATH'].split(os.pathsep) + conf.env.EXTRA_PATH
    paths = [os.path.normpath(os.path.join(path, '..', 'lib')) for path in bindirs]
    paths = set([path for path in paths if os.path.isdir(path)])
    for bindir in bindirs:
        suncc = os.path.join(bindir, 'suncc')
        if os.path.isfile(suncc):
            if conf.test_suncc(suncc[:-2]+'CC'):
                conf.env.SUNCC_TARGETS += conf.get_native_suncc_target(suncc)
    conf.env.SUNCC_TARGETS.sort(key= lambda x: (x[1], x[2], x[0]))

@conf
def load_suncc(self, bindir):
    self.find_program('suncc', var='CC', path_list=[bindir])
    self.find_program('sunCC', var='CXX', path_list=[bindir])
    self.env.COMPILER_NAME='suncc'
    self.env.COMPILER_TARGET=self.env.HOST
    self.find_program('dbx', var='GDB')
    self.load('suncc suncxx')

def options(opt):
    pass

def configure(conf):
    conf.start_msg('Looking for suncc compilers')
    conf.env.append_unique('useful_defines', ['__SUNPRO_CC'])
    conf.detect_suncc()
    conf.end_msg('done')

def build(bld):
    pass

