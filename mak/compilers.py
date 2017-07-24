from waflib import Options, Utils, Configure, Logs
from waflib.Configure import conf
import os
import re
import sys
import shlex
from copy import deepcopy


class Compiler:
    ARCHS = {
        'x86':      'x86',
        'i386':     'x86',
        'i486':     'x86',
        'i586':     'x86',
        'i686':     'x86',
        'amd64':    'amd64',
        'x86_64':   'amd64',
        'x64':      'amd64',
        'arm':      'armv7a',
        #'armv4':    'armv4',
        #'armv5':    'armv5',
        'armv6':    'armv6',
        'armv7':    'armv7a',
        'armv7a':   'armv7a',
        'armv7s':   'armv7s',
        'armv7k':   'armv7k',
        'armv7l':   'armv7l',
        'arm64':    'arm64',
        'aarch64':  'arm64',
        'aarch32':  'aarch32',
        'ppc':      'ppc',
        'powerpc':  'ppc',
        'ppc64':    'ppc64',
        'powerpc64':'ppc64',
        'spu':      'spu',
        'mips':     'mips',
        'mipsel':   'mipsel',
        'mips64':   'mips64',
        'mips64el': 'mips64el',
        'mipsel64': 'mips64el',
    }
    VECTORIZED_FLAGS = {}

    def __init__(self, compiler_c, compiler_cxx, version, platform, arch,
                 extra_args = {}, extra_env={}):
        def to_number(version_string):
             v = version_string.split('-')[0].split('.')
             result = [0, 0, 0]
             for i in (0, 1, 2):
                 if not v:
                     break
                 d = v.pop(0)
                 try:
                     result[i] = int(d)
                 except ValueError:
                    try:
                        result[i] = int(re.match('\\d+', d).group())
                    except AttributeError:
                        pass
             return tuple(result)
        self.compiler_c = compiler_c
        self.compiler_cxx = compiler_cxx
        self.defines = []
        self.extra_args = deepcopy(extra_args)
        self.version = version
        self.version_number = to_number(version)
        self.platform = platform
        self.arch = self.to_target_arch(arch)
        self.arch_name = self.arch
        self.siblings = [self]
        self.env = os.environ.copy()
        for env_name, env_value in extra_env.items():
            self.env[env_name] = env_value
        self.directories = [os.path.dirname(compiler_c)]

    def load_tools(self, conf, platform):
        conf.env.CC = self.compiler_c
        conf.env.CXX = self.compiler_cxx
        conf.load(self.TOOLS)

    def add_flags(self, compiler, flags):
        try:
            self.extra_args[compiler] += Utils.to_list(flags)[:]
        except KeyError:
            self.extra_args[compiler] = Utils.to_list(flags)[:]

    @classmethod
    def to_target_arch(self, arch):
        return self.ARCHS.get(arch, 'unknown')

    @classmethod
    def run(self, cmd, input=None, env=None):
        try:
            p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE,
                                       stdout=Utils.subprocess.PIPE,
                                       stderr=Utils.subprocess.PIPE,
                                       env=env)
            if input is not None:
                p.stdin.write(input.encode())
            out, err = p.communicate()
        except Exception as e:
            return (-1, '', str(e))
        else:
            if not isinstance(out, str):
                out = out.decode(sys.stdout.encoding, errors='ignore')
            if not isinstance(err, str):
                err = err.decode(sys.stderr.encoding, errors='ignore')
            return (p.returncode, out, err)

    def run_c(self, args, input=None):
        return self.run([self.compiler_c] + self.extra_args.get('c', []) + args, input, self.env)

    def run_cxx(self, args, input=None):
        # print(' '.join([self.compiler_cxx] + self.extra_args.get('cxx', []) + args))
        return self.run([self.compiler_cxx] + self.extra_args.get('cxx', []) + args, input, self.env)

    def sort_name(self):
        compiler_name = self.__class__.__name__.lower()
        return self.arch, compiler_name, self.version_number, self.arch_name, self.platform

    def name(self):
        compiler_name = self.NAMES[0]
        return '%s-%s-%s-%s' % (compiler_name, self.platform, self.arch_name, self.version)

    def get_multilib_compilers(self):
        return []

    def load_in_env(self, conf, platform):
        conf.env.append_unique('CFLAGS', self.extra_args.get('c', []))
        conf.env.append_unique('CXXFLAGS', self.extra_args.get('cxx', []))
        conf.env.append_unique('LINKFLAGS', self.extra_args.get('link', []))
        conf.env.TARGET_ARCH = self.arch_name
        self.set_optimisation_options(conf)
        self.set_warning_options(conf)

    def add_sibling(self, other_compiler):
        self.siblings.append(other_compiler)


class GnuCompiler(Compiler):
    ALL_ARM_ARCHS = ('armv7a', 'armv7k', 'armv7s')
    ARCH_FLAGS = {
            'arm':   ['-march=armv7-a'],
        }
    VECTORIZED_FLAGS = {
        'x86':      (('.sse3', ['-msse3', '-mssse3']),
                     ('.sse4', ['-msse4.1', '-msse4.2',]),
                     ('.avx', ['-mavx']),
                     ('.avx2', ['-mavx2']),),
        'amd64':    (('.sse3', ['-msse3', '-mssse3']),
                     ('.sse4', ['-msse4.1', '-msse4.2',]),
                     ('.avx', ['-mavx']),
                     ('.avx2', ['-mavx2']),),

        'ppc':      (('.altivec', ['-maltivec']),),
        'ppc64':    (('.altivec', ['-maltivec']),),

        'armv6' :   (('.neon', ['-mfpu=neon']),),
        'armv7a':   (('.neon', ['-mfpu=neon']),),
        'armv7s':   (('.neon', ['-mfpu=neon']),),
        'armv7k':   (('.neon', ['-mfpu=neon']),),
        'armv7l':   (('.neon', ['-mfpu=neon']),),
        'arm64':    (('.neon', []),),
    }
    MULTILIBS = {
        'x86':      ((['-m64'], 'amd64'),),
        'amd64':    ((['-m32'], 'x86'),),
        'ppc':      ((['-m64'], 'ppc64'),),
        'ppc64':    ((['-m32'], 'ppc'),),
        'mips':     ((['-m64'], 'mips64'),),
        'mipsel':   ((['-m64'], 'mips64el'),),
        'mips64':   ((['-m32'], 'mips'),),
        'mips64el': ((['-m32'], 'mipsel'),),
        'arm':      [(['-march=%s'%a], a) for a in ALL_ARM_ARCHS],
        #'armv4':    [(['-march=%s'%a], a) for a in ALL_ARM_ARCHS],
        #'armv5':    [(['-march=%s'%a], a) for a in ALL_ARM_ARCHS],
        #'armv7':    [(['-march=%s'%a], a) for a in ALL_ARM_ARCHS],
        'armv7a':   [(['-march=%s'%a], a) for a in ALL_ARM_ARCHS],
        'armv7k':   [(['-march=%s'%a], a) for a in ALL_ARM_ARCHS],
        'armv7l':   [(['-march=%s'%a], a) for a in ALL_ARM_ARCHS],
    }
    MACRO_ARCHS = (
        (('__x86_64__',),                                   'amd64'),
        (('__i386__',),                                     'x86'),
        (('__i486__',),                                     'x86'),
        (('__i586__',),                                     'x86'),
        (('__i686__',),                                     'x86'),
        (('__powerpc__',),                                  'ppc'),
        (('__POWERPC__',),                                  'ppc'),
        (('__powerpc__', '__powerpc64__'),                  'ppc64'),
        (('__POWERPC__', '__ppc64__'),                      'ppc64'),
        (('__mips64', '__mips', '_MIPSEL'),                 'mips64el'),
        (('__mips', '_MIPSEL'),                             'mipsel'),
        (('__mips64', '__mips'),                            'mips64'),
        (('__mips__',),                                     'mips'),
        (('__aarch64__',),                                  'aarch64'),
        (('__aarch64',),                                    'aarch64'),
        (('__aarch32__',),                                  'aarch32'),
        (('__arm__',),                                      'armv4'),
        (('__arm__', '__ARM_ARCH_5__'),                     'armv5'),
        (('__arm__', '__ARM_ARCH_6__'),                     'armv6'),
        (('__arm__', '__ARM_ARCH_6K__'),                    'armv6'),
        (('__arm__', '__ARM_ARCH_6Z__'),                    'armv6'),
        (('__arm__', '__ARM_ARCH_6KZ__'),                   'armv6'),
        (('__arm__', '__ARM_ARCH_7A__'),                    'armv7a'),
        (('__arm__', '__ARM_ARCH_7A__', '__ARM_ARCH_7K__'), 'armv7k'),
        (('__arm__', '__ARM_ARCH_7S__'),                    'armv7s'),
    )

    def __init__(self, compiler_c, compiler_cxx, extra_args={}, extra_env={}):
        extra_env = dict(extra_env)
        extra_env['LC_ALL'] = 'C'
        extra_env['LANG'] = 'C'
        self.sysroot = None
        extra_args = deepcopy(extra_args)
        version, platform, arch = self.get_version(compiler_cxx, extra_args, extra_env)
        Compiler.__init__(self, compiler_c, compiler_cxx, version,
                          platform, arch, extra_args, extra_env)
        for t in self.targets:
            target_dir = os.path.normpath(os.path.join(self.directories[0], '..', t, 'bin'))
            if os.path.isdir(target_dir):
                self.directories.append(target_dir)

    def get_version(self, compiler_c, extra_args, extra_env):
        env = os.environ.copy()
        for env_name, env_value in extra_env.items():
            env[env_name] = env_value
        def split_triple(t):
            t = t.split('-')
            return t[0], '-'.join(t[1:])
        arch = None
        platform = None
        result, out, err = self.run([compiler_c] + extra_args.get('c', []) + ['-dumpmachine'], env=env)
        self.target = out.strip()
        self.targets = (self.target, self.target.replace('-unknown', ''), self.target.replace('--', '-'))
        if self.target.find('-') != -1:
            arch, platform = split_triple(self.target)
        else:
            platform = self.target
        if arch:
            try:
                extra_args['c'] += self.ARCH_FLAGS.get(arch, [])
            except KeyError:
                extra_args['c'] = self.ARCH_FLAGS.get(arch, [])
            try:
                extra_args['cxx'] += self.ARCH_FLAGS.get(arch, [])
            except KeyError:
                extra_args['cxx'] = self.ARCH_FLAGS.get(arch, [])
            try:
                extra_args['link'] += self.ARCH_FLAGS.get(arch, [])
            except KeyError:
                extra_args['link'] = self.ARCH_FLAGS.get(arch, [])
        result, out, err = self.run([compiler_c] + extra_args.get('c', []) + ['-v', '-dM', '-E', '-'], '\n', env=env)
        macros = set([])
        if result != 0:
            raise Exception('Error running %s:\nresult: %d\nstdout: %s\nstderr: %s\n' % (compiler_c, result, out, err))
        out = err.split('\n') + out.split('\n')
        for line in out:
            for name in self.NAMES:
                if line.find('%s version ' % name.lower()) != -1:
                    words = line.split()
                    if 'Apple' in words:
                        self.NAMES = ('Apple'+self.NAMES[0],) + self.NAMES
                    while words[0] != name.lower() and words[1] != 'version':
                        words.pop(0)
                    version = words[2].split('-')[0]
            if line.find('Apple LLVM version ') != -1:
                words = line.split()
                while words[0] != 'Apple' and words[1] != 'LLVM' and words[2] != 'version':
                    words.pop(0)
                version = words[3].split('-')[0]
                self.NAMES = ('Apple'+self.NAMES[0],) + self.NAMES
            if line.startswith('#define'):
                macro = line.split()[1].strip()
                macros.add(macro)
            sysroot = line.find('-isysroot')
            if sysroot != -1:
                sysroot = shlex.split(line[sysroot:].replace('\\', '\\\\'))[1]
                self.sysroot = os.path.normpath(sysroot)
                self.NAMES = ('cross_' + self.NAMES[0],) + self.NAMES

        best = 0
        for values, a in self.MACRO_ARCHS:
            for v in values:
                if v not in macros:
                    break
            else:
                if len(values) > best:
                    best = len(values)
                    arch = a
        if not best:
            raise Exception('could not find architecture')
        return version, platform, arch

    def get_multilib_compilers(self):
        try:
            multilibs = self.MULTILIBS[self.arch]
        except KeyError:
            return []
        else:
            result = []
            for multilib in multilibs:
                try:
                    c = self.__class__(self.compiler_c, self.compiler_cxx,
                                       { 'c': multilib[0][:],
                                         'cxx': multilib[0][:],
                                         'link': multilib[0][:],})
                    result.append(c)
                except Exception as e:
                    pass
            return result

    def is_valid(self, conf, extra_flags = []):
        node = conf.bldnode.make_node('main.cxx')
        tgtnode = node.change_ext('')
        node.write('int main() {}\n')
        try:
            result, out, err = self.run_cxx([node.abspath(), '-c', '-o', tgtnode.abspath()] + extra_flags)
        except Exception as e:
            print(e)
            return False
        finally:
            node.delete()
            tgtnode.delete()
        return result == 0

    def set_optimisation_options(self, conf):
        v = conf.env
        v.CFLAGS_debug = ['-pipe', '-g', '-D_DEBUG'] + v.CFLAGS_debug
        v.CXXFLAGS_debug = ['-pipe', '-g', '-D_DEBUG'] + v.CXXFLAGS_debug
        v.ASFLAGS_debug = ['-pipe', '-g', '-D_DEBUG'] + v.ASFLAGS_debug
        v.LINKFLAGS_debug = ['-pipe', '-g'] + v.LINKFLAGS_debug

        v.CFLAGS_profile = ['-pipe', '-g', '-DNDEBUG', '-O3'] + v.CFLAGS_profile
        v.CXXFLAGS_profile = ['-pipe', '-Wno-unused-parameter', '-g', '-DNDEBUG', '-O3', '-fno-rtti', '-fno-exceptions'] + v.CXXFLAGS_profile
        v.ASFLAGS_profile = ['-pipe', '-g', '-DNDEBUG', '-O3'] + v.ASFLAGS_profile
        v.LINKFLAGS_profile = ['-pipe', '-g'] + v.LINKFLAGS_profile

        v.CXXFLAGS_exception = ['-fexceptions']
        v.CXXFLAGS_rtti = ['-frtti']

        v.CFLAGS_final = ['-pipe', '-g', '-DNDEBUG', '-O3'] + v.CFLAGS_final
        v.CXXFLAGS_final = ['-pipe', '-Wno-unused-parameter', '-g', '-DNDEBUG', '-O3', '-fno-rtti', '-fno-exceptions'] + v.CXXFLAGS_final
        v.ASFLAGS_final = ['-pipe', '-g', '-DNDEBUG', '-O3'] + v.ASFLAGS_final
        v.LINKFLAGS_final = ['-pipe', '-g'] + v.LINKFLAGS_final

    def set_warning_options(self, conf):
        v = conf.env
        v.CFLAGS_warnnone = ['-w'] + v.CFLAGS_warnnone
        v.CXXFLAGS_warnnone = ['-w'] + v.CXXFLAGS_warnnone
        v.CFLAGS_warnall = ['-std=c99', '-Wall', '-Wextra', '-pedantic', '-Winline', '-Werror', '-Wstrict-aliasing'] + v.CFLAGS_warnall
        v.CXXFLAGS_warnall = ['-Wall', '-Wextra', '-Werror', '-Wno-sign-compare',
                              '-Woverloaded-virtual', '-Wno-invalid-offsetof', '-Wstrict-aliasing'] + v.CXXFLAGS_warnall

    def find_target_program(self, conf, platform, program, mandatory=False):
        sys_dirs = self.directories + platform.directories
        d, a = os.path.split(self.directories[0])
        while a:
            pd = os.path.join(d, 'bin')
            if os.path.isdir(pd):
                sys_dirs.append(pd)
            d, a = os.path.split(d)

        var = program.upper()
        for t in self.targets:
            if conf.find_program('%s-%s' % (t, program), var=var, path_list=sys_dirs, mandatory=False):
                break
        else:
            for t in self.targets:
                if conf.find_program('%s-%s' % (t, program), var=var, mandatory=False):
                    break
            else:
                conf.find_program(program, var=var, path_list=sys_dirs, mandatory=mandatory)

    def error_flag(self):
        return ['-Werror']

    def load_tools(self, conf, platform):
        self.find_target_program(conf, platform, 'ar')
        self.find_target_program(conf, platform, 'strip')
        self.find_target_program(conf, platform, 'objcopy')
        self.find_target_program(conf, platform, 'gdb')
        if not conf.env.GDB:
            conf.find_program('gdb', var='GDB', mandatory=False)
        Compiler.load_tools(self, conf, platform)

    def load_in_env(self, conf, platform):
        env = conf.env
        env.IDIRAFTER = '-idirafter'
        Compiler.load_in_env(self, conf, platform)
        env.SYSROOT = env.SYSROOT or self.sysroot

        conf.end_msg('')
        conf.start_msg('      `- [cpu compute variants]')
        for variant_name, flags in self.VECTORIZED_FLAGS.get(self.arch, []):
            if self.is_valid(conf, flags + self.error_flag()):
                conf.env.append_unique('KERNEL_OPTIM_VARIANTS', [variant_name])
                conf.env['CFLAGS_%s' % variant_name] = flags
                conf.env['CXXFLAGS_%s' % variant_name] = flags
                Logs.pprint('GREEN', '+%s' % variant_name, sep=' ')
            else:
                Logs.pprint('YELLOW', '-%s' % variant_name, sep=' ')

        env.COMPILER_NAME = self.__class__.__name__.lower()
        env.COMPILER_TARGET = self.arch + '-' + self.platform
        env.CC_CPP = [env.CC, '-x', 'c', '-E']
        env.CC_CPP_SRC_F = ''
        env.CC_CPP_TGT_F = ['-o']
        self.populate_useful_variables(conf, env.SYSROOT)

    def populate_useful_variables(self, conf, sysroot):
        env = conf.env
        sysroot_flags = sysroot and ['--sysroot', sysroot] or []
        result, out, err = self.run_cxx(sysroot_flags + ['-x', 'c++', '-v', '-dM', '-E', '-'], '\n')
        result = 0
        if result != 0:
            print('could not retrieve system includes: %s' % err)
        else:
            out = out.split('\n')
            while out:
                line = out.pop(0)
                if line.startswith('#include <...>'):
                    while out:
                        path = out.pop(0).strip()
                        if path[0] != ' ':
                            break
                        env.append_unique('SYSTEM_INCLUDES', [os.path.normpath(path)])
                elif line.startswith('#define'):
                    line = line[len('#define'):].strip()
                    space = line.find(' ')
                    if space != -1:
                        define = line[:space]
                        value = line[space+1:]
                        if define in self.DEFINES:
                            env.append_unique('SYSTEM_DEFINES', ['%s=%s'%(define, value)])
                    elif line in self.DEFINES:
                        conf.env.append_unique('SYSTEM_DEFINES', [line])

        result, out, err = self.run_cxx(sysroot_flags + ['-x', 'c++', '-print-search-dirs'])
        if result != 0:
            print('could not retrieve system defines: %s' % str(e))
        else:
            out = out.split('\n')
            while out:
                line = out.pop(0)
                if line and line.startswith('libraries:'):
                    line = line[10:].strip()
                    libs = self.split_path_list(line)
            env.append_unique('SYSTEM_LIBPATHS', libs)
        self.set_warning_options(conf)
        self.set_optimisation_options(conf)

    def split_path_list(self, line):
        return line.split(os.pathsep)


Configure.ConfigurationContext.Compiler = Compiler
Configure.ConfigurationContext.GnuCompiler = GnuCompiler

@conf
def detect_executable(conf, program_name, path_list=[]):
    program = conf.find_program(program_name, var='TEMP_PROGRAM',
                            path_list=path_list, mandatory=False)
    del conf.env['TEMP_PROGRAM']
    return program


def options(opt):
    gr = opt.add_option_group('configure options')
    gr.add_option( '--compilers',
                    action='store',
                    default='',
                    dest='compilers',
                    help='List of compilers to configure for')
    for path in opt.path.make_node('compiler').listdir():
        if path.endswith('.py'):
            opt.recurse('compiler/%s'%path)


def configure(conf):
    conf.compilers = []
    compilers = Options.options.compilers
    compilers = compilers.split(',') if compilers else []
    for path in conf.path.make_node('compiler').listdir():
        if path.endswith('.py'):
            if not compilers or path[:-3] in compilers:
                conf.recurse('compiler/%s'%path)
    conf.compilers.sort(key = lambda x: x.sort_name())


def build(bld):
    compilers = Options.options.compilers
    compilers = compilers.split(',') if compilers else []
    for path in bld.path.make_node('compiler').listdir():
        if path.endswith('.py'):
            if not compilers or path[:-3] in compilers:
                bld.recurse('compiler/%s'%path)
