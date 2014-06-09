from waflib import Utils, Logs
from waflib.Configure import conf
import os, sys


@conf
def get_native_icc_target(conf, icc):
    cmd = [icc, '-V']
    try:
        p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE)
        out = p.communicate()[1]
    except:
        return (None, None)

    if not isinstance(out, str):
        out = out.decode(sys.stdout.encoding)
    out = out.split('\n')
    arch = 'x86'
    version = None
    for line in out:
        words = line.split()
        while len(words):
            if words[0] == 'Intel(R)' and words[1] == '64':
                arch = 'amd64'
            if words[0] == 'ID:':
                if words[1].startswith('m_cproc_'):
                    version = words[1][10:].split('.')[0]
            if words[0] == 'Version':
                version = words[1].split('.')[0]
            words = words[1:]
    return (arch, version)


@conf
def detect_icc(conf):
    bindirs = os.environ['PATH'].split(os.pathsep) + conf.env.EXTRA_PATH
    for bindir in bindirs:
        icc = conf.find_program('icc', path_list=[bindir], mandatory=False)
        icpc = conf.find_program('icpc', path_list=[bindir], mandatory=False)
        conf.env.ICC = []
        conf.env.ICPC = []
        if icc and icpc:
            arch, version = conf.get_native_icc_target(icc)
            conf.env.ICC_TARGETS.append(('icc', bindir, icc, icpc, version, os.uname()[0].lower(), arch, []))


@conf
def load_icc(self, bindir, icc, icpc, version, target, arch, options):
    self.env.CC = icc
    self.env.CXX = icpc
    self.env.COMPILER_NAME='icc'
    self.env.COMPILER_TARGET=target
    self.find_program('gdb', mandatory=False)
    self.load('icc icpc')

    cmd = self.env.CC + ['-x', 'c++', '-print-search-dirs']
    try:
        p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE)
        p.stdin.write('\n'.encode())
        out = p.communicate()[0]
    except Exception as e:
        print('could not retrieve system defines: %s' % str(e))
    else:
        if not isinstance(out, str):
            out = out.decode(sys.stdout.encoding)
        out = out.split('\n')
        while out:
            line = out.pop(0)
            if line and line.startswith('libraries:'):
                line = line[10:].strip()
                self.env.append_unique('SYSTEM_LIBPATHS', line.split(':'))


def options(opt):
    pass

def configure(conf):
    conf.start_msg('Looking for intel compilers')
    conf.env.append_unique('useful_defines', ['__INTEL_COMPILER', '__GNUC__', '_MSC_VER'])
    conf.env.ICC_TARGETS = []
    conf.detect_icc()
    conf.env.ICC_TARGETS.sort(key= lambda x: (x[2], x[3], x[0]))
    conf.end_msg('done')
