from waflib import Utils, Logs, Configure
import os
import sys


class ICC(Configure.ConfigurationContext.GnuCompiler):
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
    VECTORIZED_FLAGS = {
        'x86':      (('.sse3', ['-mssse3']),
                     ('.sse4', ['-msse4.2',]),
                     ('.avx', ['-mavx']),
                     ('.avx2', ['-march=core-avx2']),),
        'amd64':    (('.sse3', ['-mssse3']),
                     ('.sse4', ['-msse4.2',]),
                     ('.avx', ['-mavx']),
                     ('.avx2', ['-march=core-avx2']),),
    }
    def __init__(self, icc, icpc, extra_args={}, extra_env={}):
        Configure.ConfigurationContext.GnuCompiler.__init__(self, icc, icpc, extra_args, extra_env)

    def get_version(self, icc, extra_args, extra_env):
        env = os.environ.copy()
        for env_name, env_value in extra_env.items():
            env[env_name] = env_value
        result, out, err = self.run([icc] + extra_args.get('c', []) + ['-dM', '-E', '-'], '', env=env)
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
        self.targets = (self.target,)
        return version, platform, arch

    def set_optimisation_options(self, conf):
        Configure.ConfigurationContext.GnuCompiler.set_optimisation_options(self, conf)

    def set_warning_options(self, conf):
        Configure.ConfigurationContext.GnuCompiler.set_warning_options(self, conf)
        conf.env.append_unique('CXXFLAGS_warnall', ['-wd597'])

    def load_in_env(self, conf, platform):
        Configure.ConfigurationContext.GnuCompiler.load_in_env(self, conf, platform)
        v = conf.env
        if platform.NAME == 'Linux':
            if self.arch == 'x86':
                v.append_unique('SYSTEM_LIBPATHS', ['=/usr/lib/i386-linux-gnu'])
            elif self.arch == 'amd64':
                v.append_unique('SYSTEM_LIBPATHS', ['=/usr/lib/x86_64-linux-gnu'])
        v.append_unique('CPPFLAGS', ['-fPIC'])
        v.append_unique('CFLAGS', ['-fPIC'])
        v.append_unique('CXXFLAGS', ['-fPIC'])
        if platform.NAME != 'windows':
            if self.version_number >= (10,):
                v.append_unique('LINKFLAGS', ['-static-intel'])
            else:
                v.append_unique('LINKFLAGS', ['-i-static'])
        if self.version_number >= (11,):
            v.append_unique('CFLAGS', ['-fvisibility=hidden'])
            v.append_unique('CXXFLAGS', ['-fvisibility=hidden'])
            v.CFLAGS_exportall = ['-fvisibility=default']
            v.CXXFLAGS_exportall = ['-fvisibility=default']

    def is_valid(self, conf, extra_flags=[]):
        node = conf.bldnode.make_node('main.cxx')
        tgtnode = node.change_ext('')
        node.write('#include <iostream>\nint main() {}\n')
        try:
            result, out, err = self.run_cxx([node.abspath(), '-c', '-o', tgtnode.abspath()] + extra_flags)
        except Exception as e:
            return False
        finally:
            node.delete()
            tgtnode.delete()
        if not result:
            err = err.split('\n')
            for e in err:
                if e.find('command line warning') != -1:
                    result = 1
        return result == 0

def detect_icc(conf):
    environ = getattr(conf, 'environ', os.environ)
    bindirs = environ['PATH'].split(os.pathsep) + conf.env.EXTRA_PATH
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


def build(bld):
    pass
