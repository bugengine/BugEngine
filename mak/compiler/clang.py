from waflib import Utils, Logs
from waflib.Configure import conf
import os
import sys
import shlex

all_clang_archs = [
    ('x86', ['x86', 'i386']),
    ('amd64', ['x86_64', 'amd64', 'x64']),
    ('arm', ['arm', 'armv6', 'armv7']),
    ('arm64', ['arm64', 'aarch64']),
    ('ppc', ['ppc', 'powerpc']),
    ('ppc64', ['ppc64', 'powerpc64']),
    ('mips', ['mips']),
    ('mipsel', ['mipsel']),
    ('mips64', ['mips64']),
    ('mipsel64', ['mips64el', 'mipsel64']),
]
all_valid_clang_archs = [i for _,l in all_clang_archs for i in l]


def expand_clang_archs(arch):
    if arch in ('x86', 'i686', 'i586', 'i486', 'i386'):
        return [(['-m64'], 'amd64'), ([], 'x86')]
    elif arch in ('amd64', 'x64', 'x86_64'):
        return [(['-m32'], 'x86'), ([], 'amd64')]
    elif arch in ('arm', 'armv6', 'armv7'):
        return [([], arch)]
    elif arch == 'aarch64':
        return [([], 'arm64')]
    elif arch == 'arm64':
        return [(['-miphoneos-version-min=7.0'], 'arm64')]
    elif arch == 'mips':
        return [(['-m64'], 'mips64'), ([], 'mips')]
    elif arch == 'mips64':
        return [(['-m32'], 'mips'), ([], 'mips64')]
    elif arch == 'mipsel':
        return [(['-m64'], 'mipsel64'), ([], 'mipsel')]
    elif arch == 'mipsel64' or arch == 'mips64el':
        return [(['-m32'], 'mipsel'), ([], arch)]
    elif arch == 'powerpc' or arch == 'ppc':
        return [(['-m64'], arch+'64'), ([], arch)]
    elif arch == 'powerpc64' or arch == 'ppc64':
        return [(['-m32'], arch[:-2]), ([], arch)]
    elif arch == 'spu':
        return [[], 'spu']
    else:
        Logs.warn('unknown architecture : %s' % arch)
        return [([], arch)]


def unique_arch(arch):
    if arch in ('x86', 'i686', 'i586', 'i486', 'i386'):
        return 'x86'
    elif arch in ('amd64', 'x64', 'x86_64'):
        return 'amd64'
    elif arch == 'aarch64':
        return 'arm64'
    elif arch == 'mipsel64' or arch == 'mips64el':
        return 'mipsel64'
    elif arch == 'powerpc' or arch == 'ppc':
        return 'powerpc'
    elif arch == 'powerpc64' or arch == 'ppc64':
        return 'powerpc64'
    elif arch == 'spu':
        return 'spu'
    elif arch in ['arm64', 'mips', 'mips64', 'mipsel', 'arm', 'armv6', 'armv7']:
        return arch
    else:
        Logs.warn('unknown architecture : %s' % arch)
        return arch


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
def get_clang_include_paths(clang,):
    cmd = [clang, '-E', '-x', 'c++', '-v', '-']
    try:
        p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.STDOUT)
        out, err = p.communicate()
    except Exception as e:
        print(e)
        return []
    else:
        if not isinstance(out, str):
            out = out.decode()
        return out.split('\n')


@conf
def find_clang_archs_multilib(self, clang, target):
    result = []
    target_tuple = target.split('-')
    arch = target_tuple[0]

    for flags, a in expand_clang_archs(arch):
        node = self.bldnode.make_node('main.c')
        tgtnode = node.change_ext('')
        node.write('int main() {}\n')
        cmd = [clang] + flags + [node.abspath(), '-o', tgtnode.abspath()]
        try:
            p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.STDOUT)
            out, err = p.communicate()
        except Exception as e:
            continue
        else:
            if p.returncode == 0:
                result.append((a, flags))
        finally:
            node.delete()
            tgtnode.delete()
    return result


@conf
def find_clang_archs_multiarch(self, clang, target):
    result = []
    seen = set([])
    target_tuple = target.split('-')
    arch = target_tuple[0]

    for line in get_clang_include_paths(clang):
        line = line.strip()
        if os.path.isdir(line):
            if os.path.split(line)[1].startswith(arch):
                line = os.path.split(line)[0]
                for x in os.listdir(line):
                    c = x.split('-')
                    if len(c) < 2:
                        continue
                    if c[0] not in all_valid_clang_archs:
                        continue
                    if os.path.isdir(os.path.join(line, x)) and not x.startswith(arch):
                        a = unique_arch(c[0])
                        if a in seen:
                            continue
                        cmd = [clang, '-target', x, '-E', '-x', 'c++', '-v', '-']
                        try:
                            p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.STDOUT)
                            out, err = p.communicate()
                        except Exception:
                            continue
                        else:
                            if p.returncode == 0:
                                seen.add(a)
                                result.append((a, ['-target', x]))
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
    except Exception:
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
    libdirs = []
    for bindir in bindirs:
        for libdir in (os.path.join(bindir, '..', 'lib'), os.path.join(bindir, '..')):
            if os.path.isdir(libdir):
                for x in os.listdir(libdir):
                    if x.startswith('llvm'):
                        b = os.path.join(libdir, x, 'bin')
                        if os.path.isdir(b):
                            libdirs.append(b)

    conf.env.CLANG_TARGETS = []
    seen=set([])
    for path in libdirs+bindirs:
        clang =  conf.find_program('clang', var='CLANG', path_list=[path], mandatory=False)
        clangxx = conf.find_program('clang++', var='CLANGXX', path_list=[path], mandatory=False)
        if clang and clangxx:
            clang = os.path.normpath(os.path.realpath(clang))
            clangxx = os.path.normpath(os.path.realpath(clangxx))
            if clang in seen:
                del conf.env['CLANG']
                del conf.env['CLANGXX']
                continue
            seen.add(clang)
            toolchaindir = os.path.split(conf.env.CLANG)[0]
            target, version = conf.get_native_clang_target(conf.env.CLANG)
            if target != None and version != None:
                if conf.has_arch_flag(clang):
                    for arch in conf.find_clang_archs(conf.env.CLANG):
                        conf.env.CLANG_TARGETS.append((version, toolchaindir, target, arch))
                else:
                    archs = conf.find_clang_archs_multiarch(conf.env.CLANG, target)
                    if archs:
                        for arch in archs:
                            conf.env.CLANG_TARGETS.append((version, toolchaindir, target, arch))
                    else:
                        archs = conf.find_clang_archs_multilib(conf.env.CLANG, target)
                        for arch in archs:
                            conf.env.CLANG_TARGETS.append((version, toolchaindir, target, arch))
        del conf.env['CLANG']
        del conf.env['CLANGXX']
    conf.env.CLANG_TARGETS.sort(key = lambda x: x[0])


@conf
def load_clang(conf, directory, target, flags):
    directories = [directory, os.path.join(directory, '..', target, 'bin')]
    sys_dirs = directories + [os.path.join(directory, '..', '..', 'bin')]
    conf.find_program('clang', var='CC', path_list=directories)
    conf.find_program('clang++', var='CXX', path_list=directories, mandatory=False)
    if not conf.find_program(target+'-ar', var='AR', path_list=directories, mandatory=False):
        conf.find_program('ar', var='AR', path_list=directories, mandatory=False)
    conf.find_program('lldb', var='LLDB', path_list=sys_dirs, mandatory=False)
    if not conf.find_program(target+'-gdb', var='GDB', path_list=sys_dirs, mandatory=False):
        if not conf.find_program(target+'-gdb', var='GDB', mandatory=False):
            conf.find_program('gdb', var='GDB', path_list=sys_dirs, mandatory=False)
    conf.env.COMPILER_NAME='clang'
    conf.env.COMPILER_TARGET=target
    conf.load('gcc gxx')
    if target.find('mingw') != -1 or target.find('windows') != -1 or target.find('win32') != -1:
        winres = conf.find_program(target + '-windres', var='WINRC', path_list=[directory], mandatory=False)
        if not winres:
            winres = conf.find_program('windres', var='WINRC', path_list=[directory], mandatory=False)
        if not winres:
            winres = conf.find_program('windres', var='WINRC', mandatory=False)
        conf.load('winres')

    cmd = conf.env.CC + flags + ['-x', 'c++', '-v', '-Wp,-v', '-E', '-']
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
            sysroot = line.find('-isysroot')
            if sysroot != -1:
                sysroot = shlex.split(line[sysroot:].replace('\\', '\\\\'))[1]
                conf.env.SYSROOT = os.path.normpath(sysroot)
            elif line.startswith('#include <...>'):
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
