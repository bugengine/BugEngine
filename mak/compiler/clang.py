from waflib import Utils, Logs
from waflib.Configure import conf
import os, sys

all_clang_archs = [
    ('x86', ['x86', 'i386']),
    ('amd64', ['x86_64', 'amd64', 'x64']),
    ('arm', ['armv7']),
    ('arm64', ['arm64']),
    ('ppc', ['ppc', 'powerpc']),
    ('ppc64', ['ppc64', 'powerpc64']),
    ('mips', ['mips']),
    ('mipsel', ['mipsel']),
    ('mips64', ['mips64']),
    ('mipsel64', ['mips64el', 'mipsel64']),
]
def expand_clang_archs(arch):
    if arch in ('x86', 'i686', 'i586', 'i486', 'i386'):
        return [(['-m64'], 'amd64'), (['-m32'], 'x86')]
    elif arch in ('amd64', 'x64', 'x86_64'):
        return [(['-m32'], 'x86'), (['-m64'], 'amd64')]
    elif arch == 'arm':
        return [([], 'armv7')]
    elif arch == 'arm64':
        return [(['-miphoneos-version-min=7.0'], 'arm64')]
    elif arch == 'mips':
        return [(['-m64'], 'mips64'), (['-m32'], 'mips')]
    elif arch == 'mips64':
        return [(['-m32'], 'mips'), (['-m64'], 'mips64')]
    elif arch == 'mipsel':
        return [(['-m64'], 'mipsel64'), (['-m32'], 'mipsel')]
    elif arch == 'mipsel64':
        return [(['-m32'], 'mipsel'), (['-m64'], 'mipsel64')]
    elif arch == 'powerpc' or arch == 'ppc':
        return [(['-m64'], arch+'64'), (['-m32'], arch)]
    elif arch == 'powerpc64' or arch == 'ppc64':
        return [(['-m32'], arch[:-2]), (['-m64'], arch)]
    elif arch == 'spu':
        return [[], 'spu']
    else:
        Logs.warn('unknown architecture : %s' % arch)
        return [([], arch)]

@conf
def is_clang_working(conf, clang, options):
    cmd = [clang, '-x', 'c++', '-E', '-']
    p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE)
    p.stdin.write('#include <iostream>\n#include <cstring>\n'.encode())
    out = p.communicate()[0]
    return p.returncode == 0

@conf
def has_arch_flag(self, clang):
    cmd = [clang, '-arch', 'no arch of that name', '-E', '-']
    p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE)
    p.stdin.write('\n'.encode())
    out = p.communicate()[0]
    # if clang manages to compile, then the -arch keyword was ignored
    return p.returncode != 0

@conf
def find_clang_archs_legacy(self, clang, target):
    result = []
    original_arch = target.split('-')[0]
    for arch_flags, arch_name in expand_clang_archs(original_arch):
        result.append((arch_name, arch_flags))
    return result

@conf
def find_clang_archs(self, clang):
    result = []
    filename = os.path.join(self.bldnode.abspath(), 'main.c')
    output = os.path.join(self.bldnode.abspath(), 'main.o')
    with open(filename, 'w') as f:
        f.write('int main(int, const char**) { return 0; }')
        for arch_name, archs in all_clang_archs:
            for arch in archs:
                cmd = [clang, '-arch', arch, '-c', filename, '-o', output]
                try:
                    p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE)
                    out,err = p.communicate()
                except Exception as e:
                    print(e)
                    continue
                else:
                    if p.returncode == 0:
                        result.append((arch_name, ['-arch', arch]))
                        break
                    else:
                        if not isinstance(err, str):
                            err = err.decode()
                        if err.find('invalid arch name') == -1:
                            if err.find('No available targets are compatible') == -1:
                                if err.find('not using the clang compiler') == -1:
                                    result.append((arch_name, ['-arch', arch]))
                                    break

    return result


@conf
def get_native_clang_target(conf, clang):
    cmd = [clang, '-v']
    try:
        p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE)
        out = p.communicate()[1]
    except:
        return (None, None)
    if p.returncode != 0:
        return (None, None)

    if not isinstance(out, str):
        out = out.decode(sys.stdout.encoding)
    out = out.split('\n')
    target = None
    version = None

    for line in out:
        if line.startswith('Target:'):
            target = line.split()[1]
        if line.find('clang version ') != -1:
            words = line.split()
            while words[0] != 'clang' and words[1] != 'version':
                words.pop(0)
            version = words[2].split('-')[0]
        if line.find('Apple LLVM version ') != -1:
            words = line.split()
            while words[0] != 'Apple' and words[1] != 'LLVM' and words[2] != 'version':
                words.pop(0)
            version = words[3].split('-')[0]
    return (target, version)


@conf
def detect_clang(conf):
    bindirs = os.environ['PATH'].split(os.pathsep) + conf.env.EXTRA_PATH
    conf.env.CLANG_TARGETS = []

    for path in bindirs:
        clang =  conf.find_program('clang', var='CLANG', path_list=[path], mandatory=False)
        clangxx = conf.find_program('clang++', var='CLANGXX', path_list=[path], mandatory=False)
        if clang and clangxx:
            toolchaindir = os.path.split(conf.env.CLANG)[0]
            target, version = conf.get_native_clang_target(conf.env.CLANG)
            if target != None and version != None:
                if conf.has_arch_flag(clang):
                    for arch in conf.find_clang_archs(conf.env.CLANG):
                        conf.env.CLANG_TARGETS.append((version, toolchaindir, target, arch))
                else:
                    for arch in conf.find_clang_archs_legacy(conf.env.CLANG, target):
                        conf.env.CLANG_TARGETS.append((version, toolchaindir, target, arch))
        del conf.env['CLANG']
        del conf.env['CLANGXX']
    conf.env.CLANG_TARGETS.sort(key = lambda x: x[0])

@conf
def load_clang(conf, directory, target, flags):
    directories = [directory, os.path.join(directory, '..', target, 'bin')]
    conf.find_program('clang', var='CC', path_list=directories)
    conf.find_program('clang++', var='CXX', path_list=directories, mandatory=False)
    if not conf.find_program(target+'-ar', var='AR', path_list=directories, mandatory=False):
        conf.find_program('ar', var='AR', path_list=directories, mandatory=False)
    conf.find_program('lldb', var='LLDB', path_list=directories, mandatory=False)
    if not conf.find_program(target+'-gdb', var='GDB', path_list=[directory], mandatory=False):
        conf.find_program('gdb', var='GDB', path_list=[directory], mandatory=False)
    conf.env.COMPILER_NAME='clang'
    conf.env.COMPILER_TARGET=target
    conf.load('gcc gxx')
    if target.find('mingw') != -1:
        winres = conf.find_program(target + '-windres', var='WINRC', path_list=[directory], mandatory=False)
        if not winres:
            winres = conf.find_program('windres', var='WINRC', path_list=[directory], mandatory=False)
        if not winres:
            winres = conf.find_program('windres', var='WINRC', mandatory=False)
        conf.load('winres')

    cmd = conf.env.CC + flags + ['-x', 'c++', '-Wp,-v', '-E', '-']
    try:
        p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE)
        p.stdin.write('\n'.encode())
        out = p.communicate()[1]
    except Exception as e:
        print('could not retrieve system includes: %s' % str(e))
    else:
        if not isinstance(out, str):
            out = out.decode(sys.stdout.encoding)
        out = out.split('\n')
        while out:
            line = out.pop(0)
            if line.startswith('#include <...>'):
                while out:
                    path = out.pop(0).strip()
                    if path.startswith('End of search list'):
                        break
                    conf.env.append_unique('SYSTEM_INCLUDES', [os.path.normpath(path)])

    cmd = conf.env.CC + flags + ['-x', 'c++', '-dM', '-E', '-']
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
            line = out.pop(0)[8:].strip()
            if line:
                space = line.find(' ')
                if space != -1:
                    define = line[:space]
                    value = line[space+1:]
                    if define in conf.env.useful_defines:
                        conf.env.append_unique('SYSTEM_DEFINES', ['%s=%s'%(define, value)])
                else:
                    if line in conf.env.useful_defines:
                        conf.env.append_unique('SYSTEM_DEFINES', [line])

    cmd = conf.env.CC + flags + ['-x', 'c++', '-print-search-dirs']
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
                # clang uses ':' even on windows :-/
                line = line[10:].strip()
                libs = []
                try:
                    while True:
                        index = line.index(':', 3)
                        libs.append(line[:index])
                        line = line[index+1:]
                except ValueError:
                    pass
                conf.env.append_unique('SYSTEM_LIBPATHS', libs)


def options(opt):
    pass

def configure(conf):
    conf.start_msg('Looking for clang compilers')
    conf.env.append_unique('useful_defines', ['__clang__', '__GNUC__', '__GNUG__'])
    conf.detect_clang()
    conf.end_msg('done')

def build(bld):
    pass
