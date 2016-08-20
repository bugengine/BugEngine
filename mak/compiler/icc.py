from waflib import Utils, Logs
from waflib.Configure import conf
from mak import compilers
import os, sys



class ICC(compilers.GnuCompiler):
    DEFINES = ['__INTEL_COMPILER', '__GNUC__', '_MSC_VER']
    NAMES = ('ICC',)
    ICC_PLATFORMS = {
        '__gnu_linux__': 'linux-gnu',
        '__APPLE__': 'darwin',
    }
    ICC_ARCHS = {
        '__i386__': 'x86',
        '__x86_64__': 'amd64',
    }
    TOOLS = 'icc icpc'

    def __init__(self, icc, icpc, extra_args = [], extra_env={}):
        super(ICC, self).__init__(icc, icpc, extra_args, extra_env)

    def get_version(self, icc, extra_args, extra_env):
        env = os.environ.copy()
        for env_name, env_value in extra_env.items():
            env[env_name] = env_value
        result, out, err = self.run([icc] + extra_args + ['-dM', '-E', '-'], '', env=env)
        if result != 0:
            raise Exception('could not run ICC %s (%s)' % (icc, err))
        for l in out.split('\n'):
            if l.startswith('#define '):
                l = l.strip()[len('#define '):]
                sp = l.find(' ')
                if sp == -1:
                    macro = l
                    value = None
                else:
                    macro = l[:sp]
                    value = l[sp+1:]
                if macro in self.ICC_PLATFORMS:
                    platform = self.ICC_PLATFORMS[macro]
                elif macro in self.ICC_ARCHS:
                    arch = self.ICC_ARCHS[macro]
                elif macro == '__INTEL_COMPILER':
                    patch = value[-1]
                    minor = value[-2]
                    major = value[:-2]
                    version = '%s.%s%s' % (major, minor, patch if patch != '0' else '')
        self.target = '%s-%s' % (arch, platform)
        return version, platform, arch

    def set_optimisation_options(self, conf):
        super(ICC, self).set_optimisation_options(conf)

    def set_warning_options(self, conf):
        super(ICC, self).set_warning_options(conf)
        conf.env.append_unique('CXXFLAGS_warnall', ['-wd597'])

    def load_in_env(self, conf, platform):
        super(ICC, self).load_in_env(conf, platform)
        v = conf.env
        v.append_unique('CFLAGS', ['-fPIC'])
        v.append_unique('CXXFLAGS', ['-fPIC'])
        if self.version_number >= 11:
            v.append_unique('CFLAGS', ['-fvisibility=hidden'])
            v.append_unique('CXXFLAGS', ['-fvisibility=hidden'])
            v.CFLAGS_exportall = ['-fvisibility=default']
            v.CXXFLAGS_exportall = ['-fvisibility=default']


def detect_icc(conf):
    bindirs = os.environ['PATH'].split(os.pathsep) + conf.env.EXTRA_PATH
    seen = set([])
    for bindir in bindirs:
        icc = conf.detect_executable('icc', path_list=[bindir])
        icpc = conf.detect_executable('icpc', path_list=[bindir])
        if icc and icpc:
            c = ICC(icc, icpc)
            if c.name() in seen:
                continue
            if not c.is_valid(conf):
                continue
            seen.add(c.name())
            conf.compilers.append(c)
            for multilib_compiler in c.get_multilib_compilers():
                if multilib_compiler.name() in seen:
                    continue
                if not multilib_compiler.is_valid(conf):
                    continue
                seen.add(multilib_compiler.name())
                conf.compilers.append(multilib_compiler)


def options(opt):
    pass


def configure(conf):
    conf.start_msg('Looking for intel compilers')
    detect_icc(conf)
    conf.end_msg('done')

