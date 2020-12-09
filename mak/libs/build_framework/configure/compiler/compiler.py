from waflib import Options, Configure, Utils
import os
import sys
import re
from copy import deepcopy
from be_typing import TYPE_CHECKING


class Compiler:
    ARCHS = {
        'x86': 'x86',
        'i386': 'x86',
        'i486': 'x86',
        'i586': 'x86',
        'i686': 'x86',
        'amd64': 'amd64',
        'x86_64': 'amd64',
        'x64': 'amd64',
        'arm': 'armv7a',
        'armv6': 'armv6',
        'armv7': 'armv7a',
        'armv7a': 'armv7a',
        'armv7s': 'armv7s',
        'armv7k': 'armv7k',
        'armv7l': 'armv7l',
        'arm64': 'arm64',
        'aarch64': 'arm64',
        'aarch32': 'aarch32',
        'ppc': 'ppc',
        'powerpc': 'ppc',
        'ppc64': 'ppc64',
        'powerpc64': 'ppc64',
        'ppc64le': 'ppc64le',
        'powerpc64le': 'ppc64le',
        'spu': 'spu',
        'mips': 'mips',
        'mipsel': 'mipsel',
        'mips64': 'mips64',
        'mips64el': 'mips64el',
        'mipsel64': 'mips64el',
        'ia64': 'ia64',
        'itanium': 'ia64',
    }
    VECTORIZED_FLAGS = {}

    def __init__(self, compiler_c, compiler_cxx, version, platform, arch, extra_args={}, extra_env={}):
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
        self.platform_name = platform.replace('-', '_')
        self.arch = self.to_target_arch(arch)
        self.arch_name = self.arch
        self.siblings = [self]
        self.extra_env = extra_env
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
    def run(self, cmd, env, input=None):
        try:
            p = Utils.subprocess.Popen(
                cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE, env=env
            )
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

    def run_c(self, args, input=None, env=None):
        return self.run([self.compiler_c] + self.extra_args.get('c', []) + self.extra_args.get('link', []) + args, env or self.env, input)

    def run_cxx(self, args, input=None, env=None):
        # print(' '.join([self.compiler_cxx] + self.extra_args.get('cxx', []) + args))
        return self.run([self.compiler_cxx] + self.extra_args.get('cxx', []) + self.extra_args.get('link', []) + args, env or self.env, input)

    def sort_name(self):
        compiler_name = self.NAMES[0].lower()
        return self.arch, compiler_name, self.version_number, self.arch_name, self.platform_name

    def name(self):
        compiler_name = self.NAMES[0]
        return '%s-%s-%s-%s' % (compiler_name, self.platform, self.arch_name, self.version)

    def get_multilib_compilers(self):
        return []

    def load_in_env(self, conf, platform):
        extra_env = list(self.extra_env.items())
        conf.env.c_env = extra_env
        conf.env.cxx_env = extra_env
        conf.env.cshlib_env = extra_env
        conf.env.cxxshlib_env = extra_env
        conf.env.cprogram_env = extra_env
        conf.env.cxxprogram_env = extra_env
        conf.env.append_unique('TARGETS', list(self.targets) + [self.target])
        conf.env.append_value('CPPFLAGS', self.extra_args.get('c', []))
        conf.env.append_value('CFLAGS', self.extra_args.get('c', []))
        conf.env.append_value('CXXFLAGS', self.extra_args.get('cxx', []))
        conf.env.append_value('LINKFLAGS', self.extra_args.get('link', []))
        conf.env.TARGET_ARCH = self.arch_name
        self.set_optimisation_options(conf)
        self.set_warning_options(conf)

    def add_sibling(self, other_compiler):
        self.siblings.append(other_compiler)


Configure.ConfigurationContext.Compiler = Compiler


@Configure.conf
def detect_executable(conf, program_name, path_list=[]):
    # type: (Configure.ConfigurationContext, str, List[str]) -> Optional[str]
    program = conf.find_program(program_name, var='TEMP_PROGRAM', path_list=path_list, mandatory=False)
    del conf.env['TEMP_PROGRAM']
    return isinstance(program, list) and program[0] or program


def configure(configuration_context):
    # type: (Configure.ConfigurationContext) -> None
    configuration_context.compilers = []
    configuration_context.recurse('gnu_compiler.py')
    compilers = Options.options.compilers
    compilers = compilers.split(',') if compilers else []
    for compiler in os.listdir(configuration_context.path.abspath()):
        if not compilers or os.path.splitext(compiler)[0] in compilers:
            configuration_context.recurse(compiler)
    configuration_context.compilers.sort(key=lambda x: x.sort_name())


if TYPE_CHECKING:
    from typing import List, Optional
