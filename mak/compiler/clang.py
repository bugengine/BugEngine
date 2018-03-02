from waflib import Utils, Logs, Configure
import os
import sys


class Clang(Configure.ConfigurationContext.GnuCompiler):
    DEFINES = ['__clang__', '__GNUC__', '__GNUG__']
    NAMES = ('Clang',)
    TOOLS = 'clang clangxx'

    def __init__(self, clang, clangxx, extra_args = {}):
        Configure.ConfigurationContext.GnuCompiler.__init__(self, clang, clangxx, extra_args)

    def has_arch_flag(self):
        # if clang manages to compile, then the -arch keyword was ignored
        returncode = self.run_c(['-arch', 'no arch of that name', '-E', '-'], '\n')[0]
        return returncode != 0

    def set_warning_options(self, conf):
        Configure.ConfigurationContext.GnuCompiler.set_warning_options(self, conf)
        if 'AppleClang' in self.NAMES:
            if self.version_number >= (6, 1):
                conf.env.CXXFLAGS_warnall.append('-Wno-unused-local-typedefs')
        else:
            if self.version_number >= (6, 0):
                conf.env.CXXFLAGS_warnall.append('-Wno-deprecated-register')
            if self.version_number >= (3, 6):
                conf.env.CXXFLAGS_warnall.append('-Wno-unused-local-typedefs')

    def get_multilib_compilers(self):
        result = []
        seen = set([self.arch])
        if self.has_arch_flag():
            for arch_target, arch_name in sorted(self.ARCHS.items()):
                if arch_name in seen:
                    continue
                try:
                    c = self.__class__(self.compiler_c, self.compiler_cxx,
                                       self.extra_args.get('c', []) + ['-arch', arch_target])
                except Exception:
                    pass
                else:
                    if c.arch in seen:
                        continue
                    result.append(c)
                    seen.add(c.arch)
        target_tuple = self.target.split('-')
        arch = target_tuple[0]
        r, out, err = self.run_cxx(['-x', 'c++', '-v', '-E', '-'], '\n')

        out = out.split('\n') + err.split('\n')
        while out:
            line = out.pop(0)
            if line.startswith('#include <...>'):
                while out:
                    path = out.pop(0)
                    if path[0] != ' ':
                        break
                    path = path.strip()
                    if os.path.isdir(path):
                        if os.path.split(path)[1].startswith(arch):
                            path = os.path.dirname(path)
                            for x in os.listdir(path):
                                c = x.split('-')
                                if len(c) < 2:
                                    continue
                                if c[0] not in self.ARCHS:
                                    continue
                                if os.path.isdir(os.path.join(path, x)) and not x.startswith(arch):
                                    a = self.to_target_arch(c[0])
                                    if a in seen:
                                        continue
                                    try:
                                        c = self.__class__(self.compiler_c, self.compiler_cxx,
                                                           {
                                                               'c': self.extra_args.get('c', []) + ['-target', x],
                                                               'cxx': self.extra_args.get('cxx', []) + ['-target', x],
                                                               'link': self.extra_args.get('link', []) + ['-target', x],
                                                           })
                                    except Exception:
                                        pass
                                    else:
                                        if c.arch in seen:
                                            continue
                                        result.append(c)
                                        seen.add(c.arch)
        if not result:
            result = Configure.ConfigurationContext.GnuCompiler.get_multilib_compilers(self)
        return result

    def load_in_env(self, conf, platform):
        Configure.ConfigurationContext.GnuCompiler.load_in_env(self, conf, platform)
        env = conf.env
        # Typedef of __float128 on older clangs
        if self.version_number < (3, 9):
            env.append_unique('CXXFLAGS', ['-include', os.path.join(conf.bugenginenode.abspath(),
                                                                    'mak/compiler/clang/float128.h')])
        # Template export was fixed in Clang 3.2
        if self.version_number >= (3, 2):
            if platform.NAME != 'windows':
                env.append_unique('CFLAGS', ['-fvisibility=hidden'])
                env.append_unique('CXXFLAGS', ['-fvisibility=hidden'])
                env.CFLAGS_exportall = ['-fvisibility=default']
                env.CXXFLAGS_exportall = ['-fvisibility=default']

    def split_path_list(self, line):
        result = []
        try:
            while True:
                index = line.index(':', 3)
                result.append(line[:index])
                line = line[index+1:]
        except ValueError:
            pass
        return result



def detect_clang(conf):
    bindirs = os.environ['PATH'].split(os.pathsep) + conf.env.EXTRA_PATH
    libdirs = []
    clangs = []
    for bindir in bindirs:
        for libdir in (os.path.join(bindir, '..', 'lib'), os.path.join(bindir, '..')):
            if os.path.isdir(libdir):
                for x in os.listdir(libdir):
                    if x.startswith('llvm'):
                        b = os.path.join(libdir, x, 'bin')
                        if os.path.isdir(b):
                            libdirs.append(b)

    seen = {}
    for path in libdirs+bindirs:
        clang =  conf.detect_executable('clang', path_list=[path])
        clangxx = conf.detect_executable('clang++', path_list=[path])
        if clang and clangxx:
            clang = os.path.normpath(clang)
            clangxx = os.path.normpath(clangxx)
            try:
                c = Clang(clang, clangxx)
            except Exception:
                pass
            else:
                if not c.is_valid(conf):
                    continue
                try:
                    seen[c.name()].add_sibling(c)
                except KeyError:
                    seen[c.name()] = c
                    conf.compilers.append(c)
                    clangs.append(c)
    for c in clangs:
        for multilib_compiler in c.get_multilib_compilers():
            if not multilib_compiler.is_valid(conf):
                continue
            try:
                seen[multilib_compiler.name()].add_sibling(multilib_compiler)
            except KeyError:
                seen[multilib_compiler.name()] = multilib_compiler
                conf.compilers.append(multilib_compiler)


def options(opt):
    pass


def configure(conf):
    conf.start_msg('Looking for clang compilers')
    detect_clang(conf)
    conf.end_msg('done')


def build(bld):
    pass
