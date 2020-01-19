from waflib import Utils
import os
import sys
from copy import deepcopy
import re


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
                                   #'armv4':    'armv4',
                                   #'armv5':    'armv5',
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
        'spu': 'spu',
        'mips': 'mips',
        'mipsel': 'mipsel',
        'mips64': 'mips64',
        'mips64el': 'mips64el',
        'mipsel64': 'mips64el',
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
            p = Utils.subprocess.Popen(cmd,
                                       stdin=Utils.subprocess.PIPE,
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

    def run_c(self, args, input=None, env=None):
        return self.run([self.compiler_c] + self.extra_args.get('c', []) + args, input, env or self.env)

    def run_cxx(self, args, input=None, env=None):
        # print(' '.join([self.compiler_cxx] + self.extra_args.get('cxx', []) + args))
        return self.run([self.compiler_cxx] + self.extra_args.get('cxx', []) + args, input, env or self.env)

    def sort_name(self):
        compiler_name = self.NAMES[0].lower()
        return self.arch, compiler_name, self.version_number, self.arch_name, self.platform

    def name(self):
        compiler_name = self.NAMES[0]
        return '%s-%s-%s-%s' % (compiler_name, self.platform, self.arch_name, self.version)

    def get_multilib_compilers(self):
        return []

    def load_in_env(self, conf, platform):
        conf.env.append_unique('TARGETS', list(self.targets) + [self.target])
        conf.env.append_unique('CPPFLAGS', self.extra_args.get('c', []))
        conf.env.append_unique('CFLAGS', self.extra_args.get('c', []))
        conf.env.append_unique('CXXFLAGS', self.extra_args.get('cxx', []))
        conf.env.append_unique('LINKFLAGS', self.extra_args.get('link', []))
        conf.env.TARGET_ARCH = self.arch_name
        self.set_optimisation_options(conf)
        self.set_warning_options(conf)

    def add_sibling(self, other_compiler):
        self.siblings.append(other_compiler)

