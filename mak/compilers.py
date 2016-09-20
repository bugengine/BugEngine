from waflib import Options, Utils, Configure
from waflib.Configure import conf
import os
import sys
import shlex


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
        'arm':      'arm',
        'armv6':    'armv6',
        'armv7':    'armv7',
        'arm64':    'aarch64',
        'aarch64':  'aarch64',
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

    def __init__(self, compiler_c, compiler_cxx, version, platform, arch,
                 extra_args = [], extra_env={}):
        def to_number(version_string):
             v = version_string.split('-')[0].split('.')
             div = 1
             result = 0
             while v:
                 d = v.pop(0)
                 try:
                     result += int(d) / div
                 except ValueError:
                     pass
                 else:
                    div = div * 10
             return result
        self.compiler_c = compiler_c
        self.compiler_cxx = compiler_cxx
        self.defines = []
        self.extra_args = extra_args[:]
        self.version = version
        self.version_number = to_number(version)
        self.platform = platform
        self.arch = self.to_target_arch(arch)
        self.arch_name = self.arch
        self.env = os.environ.copy()
        for env_name, env_value in extra_env.items():
            self.env[env_name] = env_value
        self.directories = [os.path.dirname(compiler_c)]

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
        return self.run([self.compiler_c] + self.extra_args + args, input, self.env)

    def run_cxx(self, args, input=None):
        return self.run([self.compiler_cxx] + self.extra_args + args, input, self.env)

    def name(self):
        compiler_name = self.__class__.__name__.lower()
        return '%s-%s-%s-%s' % (compiler_name, self.platform, self.arch_name, self.version)

    def get_multilib_compilers(self):
        return []

    def load_in_env(self, conf, platform):
        conf.env.append_unique('CFLAGS', self.extra_args)
        conf.env.append_unique('CXXFLAGS', self.extra_args)
        conf.env.append_unique('LINKFLAGS', self.extra_args)
        self.set_optimisation_options(conf)
        self.set_warning_options(conf)


class GnuCompiler(Compiler):
    MULTILIBS = {
        'x86':      (['-m64'], 'amd64'),
        'amd64':    (['-m32'], 'x86'),
        'ppc':      (['-m64'], 'ppc64'),
        'ppc64':    (['-m32'], 'ppc'),
        'mips':     (['-m64'], 'mips64'),
        'mipsel':   (['-m64'], 'mips64el'),
        'mips64':   (['-m32'], 'mips'),
        'mips64el': (['-m32'], 'mipsel'),
    }
    MACRO_ARCHS = {
        ('__x86_64__',):                    'amd64',
        ('__i386__',):                      'x86',
        ('__i486__',):                      'x86',
        ('__i586__',):                      'x86',
        ('__i686__',):                      'x86',
        ('__powerpc__',):                   'ppc',
        ('__powerpc__', '__powerpc64__'):   'ppc64',
        ('__mips64', '__mips', '_MIPSEL'):  'mips64el',
        ('__mips', '_MIPSEL'):              'mipsel',
        ('__mips64', '__mips'):             'mips64',
        ('__mips__',):                      'mips',
        ('__arm',):                         'arm',
        ('__aarch64',):                     'aarch64',
        ('__aarch32',):                     'aarch32',
    }

    def __init__(self, compiler_c, compiler_cxx, extra_args = [], extra_env={}):
        extra_env = dict(extra_env)
        extra_env['LC_ALL'] = 'C'
        extra_env['LANG'] = 'C'
        self.sysroot = None
        version, platform, arch = self.get_version(compiler_cxx, extra_args, extra_env)
        Compiler.__init__(self, compiler_c, compiler_cxx, version,
                          platform, arch, extra_args, extra_env)
        target_dir = os.path.normpath(os.path.join(self.directories[0], '..', self.target, 'bin'))
        if os.path.isdir(target_dir):
            self.directories.append(target_dir)

    def get_version(self, compiler_c, extra_args, extra_env):
        env = os.environ.copy()
        for env_name, env_value in extra_env.items():
            env[env_name] = env_value
        def split_triple(t):
            t = t.split('-')
            return t[0], '-'.join(t[1:])
        result, out, err = self.run([compiler_c] + extra_args + ['-v', '-dM', '-E', '-'], '\n', env=env)
        macros = set([])
        platform = None
        if result != 0:
            #print(result, out, err)
            raise Exception('Error running %s: %s' % (compiler_c, err))
        out = out.split('\n') + err.split('\n')
        for line in out:
            if line.startswith('Target:'):
                self.target = line.split()[1]
                arch, platform = split_triple(self.target)
            for name in self.NAMES:
                if line.find('%s version ' % name.lower()) != -1:
                    words = line.split()
                    while words[0] != name.lower() and words[1] != 'version':
                        words.pop(0)
                    version = words[2].split('-')[0]
            if line.find('Apple LLVM version ') != -1:
                words = line.split()
                while words[0] != 'Apple' and words[1] != 'LLVM' and words[2] != 'version':
                    words.pop(0)
                version = words[3].split('-')[0]
            if line.startswith('#define'):
                macro = line[len('#define'):].strip()
                macro = macro.split(' ')[0].strip()
                macros.add(macro)
            sysroot = line.find('-isysroot')
            if sysroot != -1:
                sysroot = shlex.split(line[sysroot:].replace('\\', '\\\\'))[1]
                self.sysroot = os.path.normpath(sysroot)
        if not platform:
            result, out, err = self.run([compiler_c] + extra_args + ['-dumpmachine'], env=env)
            platform = out.strip()
            if platform.find('-') != -1:
                arch, platform = split_triple(platform)
            best = 0
            for values, a in self.MACRO_ARCHS.items():
                for v in values:
                    if v not in macros:
                        break
                else:
                    if len(values) > best:
                        best = len(values)
                        arch = a
            if not best:
                raise Exception('could not find architecture')
            self.target = arch + '-' + platform
        return version, platform, arch

    def get_multilib_compilers(self):
        try:
            multilib = self.MULTILIBS[self.arch]
        except KeyError:
            return []
        else:
            try:
                return [self.__class__(self.compiler_c, self.compiler_cxx, multilib[0])]
            except Exception:
                return []

    def is_valid(self, conf):
        node = conf.bldnode.make_node('main.cxx')
        tgtnode = node.change_ext('')
        node.write('#include <cstdlib>\n#include <cstdio>\nint main() {}\n')
        try:
            result, out, err = self.run_cxx([node.abspath(), '-o', tgtnode.abspath()])
        except Exception as e:
            print(e)
            return False
        finally:
            node.delete()
            tgtnode.delete()
        return result == 0

    def set_optimisation_options(self, conf):
        v = conf.env
        v.CFLAGS_debug = ['-pipe', '-g', '-D_DEBUG']
        v.CXXFLAGS_debug = ['-pipe', '-g', '-D_DEBUG']
        v.ASFLAGS_debug = ['-pipe', '-g', '-D_DEBUG']
        v.LINKFLAGS_debug = ['-pipe', '-g']

        v.CFLAGS_profile = ['-pipe', '-g', '-DNDEBUG', '-O3']
        v.CXXFLAGS_profile = ['-pipe', '-Wno-unused-parameter', '-g', '-DNDEBUG', '-O3', '-fno-rtti', '-fno-exceptions']
        v.ASFLAGS_profile = ['-pipe', '-g', '-DNDEBUG', '-O3']
        v.LINKFLAGS_profile = ['-pipe', '-g']

        v.CFLAGS_final = ['-pipe', '-g', '-DNDEBUG', '-O3']
        v.CXXFLAGS_final = ['-pipe', '-Wno-unused-parameter', '-g', '-DNDEBUG', '-O3', '-fno-rtti', '-fno-exceptions']
        v.ASFLAGS_final = ['-pipe', '-g', '-DNDEBUG', '-O3']
        v.LINKFLAGS_final = ['-pipe', '-g']

    def set_warning_options(self, conf):
        v = conf.env
        v.CFLAGS_warnnone = ['-w']
        v.CXXFLAGS_warnnone = ['-w']
        v.CFLAGS_warnall = ['-std=c99', '-Wall', '-Wextra', '-pedantic', '-Winline', '-Werror']
        v.CXXFLAGS_warnall = ['-Wall', '-Wextra', '-Werror', '-Wno-sign-compare',
                              '-Woverloaded-virtual', '-Wno-invalid-offsetof']

    def load_in_env(self, conf, platform, sysroot=None):
        env = conf.env
        env.CC = self.compiler_c
        env.CXX = self.compiler_cxx
        env.SYSROOT = sysroot or self.sysroot or []
        Compiler.load_in_env(self, conf, platform)


        sys_dirs = self.directories[:]
        d, a = os.path.split(self.directories[0])
        while a:
            pd = os.path.join(d, 'bin')
            if os.path.isdir(pd):
                sys_dirs.append(pd)
            d, a = os.path.split(d)

        if not conf.find_program(self.target+'-ar', var='AR', path_list=self.directories, mandatory=False):
            conf.find_program('ar', var='AR', path_list=self.directories, mandatory=False)
        conf.find_program('lldb', var='LLDB', path_list=sys_dirs, mandatory=False)
        if not conf.find_program(self.target+'-gdb', var='GDB', mandatory=False):
            if not conf.find_program('gdb', var='GDB', path_list=sys_dirs, mandatory=False):
                conf.find_program('gdb', var='GDB', mandatory=False)
        env.COMPILER_NAME = self.__class__.__name__.lower()
        env.COMPILER_TARGET = self.target
        conf.load(self.TOOLS)
        self.populate_useful_variables(conf)

    def populate_useful_variables(self, conf):
        env = conf.env
        result, out, err = self.run_cxx(['-x', 'c++', '-v', '-dM', '-E', '-'], '\n')
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

        result, out, err = self.run_cxx(['-x', 'c++', '-print-search-dirs'])
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
    conf.compilers.sort(key = lambda x: x.name())


def build(bld):
    compilers = Options.options.compilers
    compilers = compilers.split(',') if compilers else []
    for path in bld.path.make_node('compiler').listdir():
        if path.endswith('.py'):
            if not compilers or path[:-3] in compilers:
                bld.recurse('compiler/%s'%path)
