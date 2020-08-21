from waflib import Utils, Logs, Configure, Options
from waflib.Tools import c_config, gcc, gxx
import os
import sys


@Configure.conf
def check_gcc_o_space(self, mode='c'):
    pass


@Configure.conf
def gcc_modifier_platform(conf):
    pass


@Configure.conf
def gxx_modifier_platform(conf):
    pass


class GCC(Configure.ConfigurationContext.GnuCompiler):
    DEFINES = ['__GNUC__', '__GNUG__']
    NAMES = ('GCC', )
    TOOLS = 'gcc gxx'

    def __init__(self, gcc, gxx, extra_args={}, extra_env={}):
        Configure.ConfigurationContext.GnuCompiler.__init__(self, gcc, gxx, extra_args, extra_env)

    def set_optimisation_options(self, conf):
        Configure.ConfigurationContext.GnuCompiler.set_optimisation_options(self, conf)

    def set_warning_options(self, conf):
        Configure.ConfigurationContext.GnuCompiler.set_warning_options(self, conf)
        if self.version_number >= (4, 8):
            v = conf.env
            v.CXXFLAGS_warnall.append('-Wno-unused-local-typedefs')

    def load_in_env(self, conf, platform):
        Configure.ConfigurationContext.GnuCompiler.load_in_env(self, conf, platform)
        v = conf.env
        if self.version_number < (
            4,
            3,
        ):
            v.append_unique('CFLAGS', ['-static-libgcc'])
            v.append_unique('CXXFLAGS', ['-static-libgcc'])
            v.append_unique('LINKFLAGS', ['-static-libgcc'])
        if self.version_number >= (4, ):
            if platform.NAME != 'windows':
                v.append_unique('CFLAGS', ['-fvisibility=hidden'])
                v.append_unique('CXXFLAGS', ['-fvisibility=hidden'])
                v.CFLAGS_exportall = ['-fvisibility=default']
                v.CXXFLAGS_exportall = ['-fvisibility=default']


class LLVM(GCC):
    DEFINES = ['__GNUC__', '__GNUG__']
    NAMES = ('LLVM', 'GCC')

    def __init__(self, gcc, gxx, extra_args={}):
        GCC.__init__(self, gcc, gxx, extra_args)


def detect_gcc_version(conf, bindir, version, target, seen):
    gcc_compilers = []
    v = version.split('.')
    versions = [
        '.'.join(v),
        ''.join(v),
        '.'.join(v[0:2]),
        ''.join(v[0:2]),
        v[0],
        '-' + '.'.join(v),
        '-' + ''.join(v),
        '-' + '.'.join(v[0:2]),
        '-' + ''.join(v[0:2]),
        '-' + v[0],
        '',
    ]

    def find_target_gcc(gcc_name_prefix, cls):
        cc = cxx = None
        for v in versions:
            cc = conf.detect_executable('%s-gcc%s' % (gcc_name_prefix, v), path_list=[bindir])
            if cc:
                break
        for v in versions:
            cxx = conf.detect_executable('%s-g++%s' % (gcc_name_prefix, v), path_list=[bindir])
            if cxx:
                break
        if cc and cxx:
            try:
                c = cls(cc, cxx)
            except Exception as e:
                #Logs.pprint('YELLOW', '%s: %s' % (cc, e))
                pass
            else:
                if not c.is_valid(conf):
                    return
                try:
                    seen[c.name()].add_sibling(c)
                except KeyError:
                    seen[c.name()] = c
                    conf.compilers.append(c)

                return c

    c = find_target_gcc(target, GCC)
    if c:
        gcc_compilers.append(c)
        result, out, err = c.run_c(['-fplugin=dragonegg', '-E', '-'], '')
        if result == 0:
            c = find_target_gcc('llvm', LLVM)
            if c:
                gcc_compilers.append(c)
    return gcc_compilers


def detect_gcc_from_path(conf, path, seen):
    gcc_compilers = []
    for subdir, relative in [('', '../..'), ('lib/gcc', '../../../..'), ('gcc', '../../..'), ('llvm', '../../..')]:
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
                gcc_compilers += detect_gcc_version(conf, bindir, version, target, seen)
    for version in os.listdir(path):
        if os.path.isdir(os.path.join(path, version, 'gcc')):
            for target in os.listdir(os.path.join(path, version, 'gcc')):
                bindir = os.path.normpath(os.path.join(path, '..', '..', 'bin'))
                gcc_compilers += detect_gcc_version(conf, bindir, version, target, seen)
    return gcc_compilers


def detect_multilib_compilers(conf, gcc_compilers, seen):
    multilib_compilers = []
    for c in gcc_compilers:
        for multilib_compiler in c.get_multilib_compilers():
            if not multilib_compiler.is_valid(conf):
                continue
            try:
                seen[multilib_compiler.name()].add_sibling(multilib_compiler)
            except KeyError:
                seen[multilib_compiler.name()] = multilib_compiler
                multilib_compilers.append(multilib_compiler)
    for c in gcc_compilers + multilib_compilers:
        for sysroot_path, targets in conf.env.SYSROOTS:
            for target in targets:
                if target == c.target:
                    try:
                        compiler = c.__class__(
                            c.compiler_c, c.compiler_cxx, {
                                'c': c.extra_args.get('c', []) + ['--sysroot', sysroot_path],
                                'cxx': c.extra_args.get('cxx', []) + ['--sysroot', sysroot_path],
                                'link': c.extra_args.get('link', []) + ['--sysroot', sysroot_path],
                            }
                        )
                    except Exception as e:
                        pass
                    else:
                        try:
                            seen[compiler.name()].add_sibling(compiler)
                        except KeyError:
                            seen[compiler.name()] = compiler
                            multilib_compilers.append(compiler)
    conf.compilers += multilib_compilers


def get_native_gcc(conf, seen):
    import platform
    if platform.uname()[0].lower() == 'freebsd':
        try:
            c = GCC('/usr/bin/gcc', '/usr/bin/g++')
        except BaseException:
            pass
        else:
            if c.is_valid(conf):
                try:
                    seen[c.name()].add_sibling(c)
                except KeyError:
                    seen[c.name()] = c
                    conf.compilers.append(c)
                for multilib_compiler in c.get_multilib_compilers():
                    if not multilib_compiler.is_valid(conf):
                        continue
                    try:
                        seen[multilib_compiler.name()].add_sibling(c)
                    except KeyError:
                        seen[multilib_compiler.name()] = multilib_compiler
                        conf.compilers.append(multilib_compiler)


def detect_gcc(conf):
    environ = getattr(conf, 'environ', os.environ)
    bindirs = environ['PATH'].split(os.pathsep) + conf.env.EXTRA_PATH
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
    seen = {}
    gcc_compilers = []
    for path in paths:
        try:
            for lib in os.listdir(path):
                if lib.startswith('gcc'):
                    gcc_lib_path = os.path.join(path, lib)
                    gcc_compilers += detect_gcc_from_path(conf, gcc_lib_path, seen)
                    for version in os.listdir(gcc_lib_path):
                        if os.path.isdir(os.path.join(gcc_lib_path, version, 'gcc')):
                            gcc_compilers += detect_gcc_from_path(conf, os.path.join(gcc_lib_path, version, 'gcc'), seen)
        except OSError:
            pass
    detect_multilib_compilers(conf, gcc_compilers, seen)
    get_native_gcc(conf, seen)


def configure(conf):
    conf.start_msg('Looking for gcc compilers')
    detect_gcc(conf)
    conf.end_msg('done')
