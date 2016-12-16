from waflib import Utils, Logs, Configure
import os
import sys


class GCC(Configure.ConfigurationContext.GnuCompiler):
    DEFINES = ['__GNUC__', '__GNUG__']
    NAMES = ('GCC',)
    TOOLS = 'gcc gxx'

    def __init__(self, gcc, gxx, extra_args = []):
        Configure.ConfigurationContext.GnuCompiler.__init__(self, gcc, gxx, extra_args)

    def set_warning_options(self, conf):
        Configure.ConfigurationContext.GnuCompiler.set_warning_options(self, conf)
        if self.version_number >= 4.8:
            v.CXXFLAGS_warnall.append('-Wno-unused-local-typedefs')

    def set_optimisation_options(self, conf):
        Configure.ConfigurationContext.GnuCompiler.set_optimisation_options(self, conf)

    def set_warning_options(self, conf):
        Configure.ConfigurationContext.GnuCompiler.set_warning_options(self, conf)
        if self.version_number >= 4.8:
            v = conf.env
            v.CXXFLAGS_warnall.append('-Wno-unused-local-typedefs')

    def load_in_env(self, conf, platform, sysroot=None):
        Configure.ConfigurationContext.GnuCompiler.load_in_env(self, conf, platform, sysroot)
        if self.version_number >= 4:
            if platform.NAME != 'windows':
                v = conf.env
                v.append_unique('CFLAGS', ['-fvisibility=hidden'])
                v.append_unique('CXXFLAGS', ['-fvisibility=hidden'])
                v.CFLAGS_exportall = ['-fvisibility=default']
                v.CXXFLAGS_exportall = ['-fvisibility=default']


class LLVM(GCC):
    DEFINES = ['__GNUC__', '__GNUG__']
    NAMES = ('LLVM', 'GCC')

    def __init__(self, gcc, gxx, extra_args = []):
        GCC.__init__(self, gcc, gxx, extra_args)


def detect_gcc_from_path(conf, path, seen):
    for subdir, relative in [('', '../..'), ('lib/gcc', '../../../..'), ('gcc', '../../..'),
                             ('llvm', '../../..')]:
        libdir = os.path.join(path, subdir)
        if not os.path.isdir(libdir):
            continue
        for target in os.listdir(libdir):
            if not os.path.isdir(os.path.join(libdir, target)):
                continue
            if target in ['.svn', '.cvs']:
                continue
            for version in os.listdir(os.path.join(libdir, target)):
                if not os.path.isdir(os.path.join(libdir, target, version)):
                    continue
                if version in ['.svn', '.cvs']:
                    continue
                if os.path.islink(os.path.join(libdir, target, version)):
                    continue
                bindir = os.path.normpath(os.path.join(libdir, relative, 'bin'))
                v = version.split('.')
                versions = [
                    '.'.join(v),
                    ''.join(v),
                    '.'.join(v[0:2]),
                    ''.join(v[0:2]),
                    v[0],
                    '-'+'.'.join(v),
                    '-'+''.join(v),
                    '-'+'.'.join(v[0:2]),
                    '-'+''.join(v[0:2]),
                    '-'+v[0],
                    '',
                ]

                def find_target_gcc(gcc_name_prefix, cls):
                    cc = cxx = None
                    for v in versions:
                        cc = conf.detect_executable('%s-gcc%s'%(gcc_name_prefix, v), path_list=[bindir])
                        if cc:
                            break
                    for v in versions:
                        cxx = conf.detect_executable('%s-g++%s'%(gcc_name_prefix, v), path_list=[bindir])
                        if cxx:
                            break
                    if cc and cxx:
                        try:
                            c = cls(cc, cxx)
                        except Exception as e:
                            Logs.pprint('YELLOW', '%s: %s' % (cc, e))
                        else:
                            if c.name() in seen:
                                return
                            if not c.is_valid(conf):
                                return
                            seen.add(c.name())
                            conf.compilers.append(c)
                            for multilib_compiler in c.get_multilib_compilers():
                                if multilib_compiler.name() in seen:
                                    continue
                                if not multilib_compiler.is_valid(conf):
                                    continue
                                seen.add(multilib_compiler.name())
                                conf.compilers.append(multilib_compiler)
                            return c
                c = find_target_gcc(target, GCC)
                if c:
                    result, out, err = c.run_c(['-fplugin=dragonegg', '-E', '-'], '')
                    if result == 0:
                        find_target_gcc('llvm', LLVM)


def get_native_gcc(conf, seen):
    import platform
    if platform.uname()[0].lower() == 'freebsd':
        c = GCC('/usr/bin/gcc', '/usr/bin/g++')
        if c.name() not in seen and c.is_valid(conf):
            seen.add(c.name())
            for multilib_compiler in c.get_multilib_compilers():
                if multilib_compiler.name() in seen:
                    continue
                if not multilib_compiler.is_valid(conf):
                    continue
                seen.add(multilib_compiler.name())
                conf.compilers.append(multilib_compiler)


def detect_gcc(conf):
    bindirs = os.environ['PATH'].split(os.pathsep) + conf.env.EXTRA_PATH
    paths = [os.path.normpath(os.path.join(path, '..', 'lib')) for path in bindirs]
    paths = set([path for path in paths if os.path.isdir(path)])
    for bindir in bindirs:
        try:
            for f in os.listdir(os.path.join(bindir, '..')):
                if os.path.isdir(os.path.join(bindir, '..', f, 'lib')):
                    paths.add(os.path.normpath(os.path.join(bindir, '..', f, 'lib')))
        except OSError:
            pass
        try:
            for f in os.listdir(os.path.join(bindir, '..', 'lib', 'llvm')):
                if os.path.isdir(os.path.join(bindir, '..', 'lib', 'llvm', f, 'lib')):
                    paths.add(os.path.normpath(os.path.join(bindir, '..', 'lib', 'llvm', f, 'lib')))
        except OSError:
            pass
    paths = paths.union(conf.env.ALL_ARCH_LIBPATHS)
    seen = set([])
    for path in paths:
        try:
            for lib in os.listdir(path):
                if lib.startswith('gcc'):
                    detect_gcc_from_path(conf, os.path.join(path, lib), seen)
        except OSError:
            pass
    get_native_gcc(conf, seen)


def options(opt):
    pass


def configure(conf):
    conf.start_msg('Looking for gcc compilers')
    detect_gcc(conf)
    conf.end_msg('done')


def build(bld):
    pass
