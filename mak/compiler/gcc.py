from waflib import Utils, Logs
from waflib.Configure import conf
import os
import sys
import shlex

def allarchs(arch):
    if arch == 'x86':
        return [(['-m64'], 'amd64'), (['-m32'], 'x86')]
    elif arch == 'amd64':
        return [(['-m32'], 'x86'), (['-m64'], 'amd64')]
    elif arch == 'arm64':
        return [([], 'arm64')]
    elif arch in ('arm', 'armv6', 'armv7'):
        return [([], arch)]
    elif arch == 'mips':
        return [(['-m64'], 'mips64'), (['-m32'], 'mips')]
    elif arch == 'mips64':
        return [(['-m32'], 'mips'), (['-m64'], 'mips64')]
    elif arch == 'mipsel':
        return [(['-m64'], 'mipsel64'), (['-m32'], 'mipsel')]
    elif arch == 'mipsel64' or arch=='mips64el':
        return [(['-m32'], 'mipsel'), (['-m64'], 'mipsel64')]
    elif arch == 'powerpc' or arch == 'ppc':
        return [(['-m64'], arch+'64'), (['-m32'], arch)]
    elif arch in ['ppu']:
        return [(['-m32'], arch), (['-m64'], arch+'64')]
    elif arch in ['powerpc64', 'ppc64']:
        return [(['-m32'], arch[:-2]), (['-m64'], arch)]
    elif arch == 'spu':
        return [([], 'spu')]
    else:
        Logs.warn('unknown architecture : %s' % arch)
        return [([], arch)]


@conf
def get_gcc_version_float(self, version):
    version = version.split('-')[0]
    version_number = 0.0
    div = 1.0
    for i in version.split('.'):
        try:
            version_number += float(i)*div
            div = div*0.1
        except ValueError:
            pass
    return version_number


@conf
def parse_gcc_target(conf, target):
    for gccname,aname in conf.gcc_architectures:
        if target.find(gccname) != -1:
            return aname

@conf
def add_all_archs_to_env(conf, name, bindir, cc, cxx, version, target, arch):
    if cc and cxx:
        found = False
        for gcc_option, gcc_arch in allarchs(arch):
            node = conf.bldnode.make_node('main.c')
            tgtnode = node.change_ext('')
            node.write('int main() {}\n')
            cmd = [cc] + gcc_option + [node.abspath(), '-o', tgtnode.abspath()]
            try:
                p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE)
                out,err = p.communicate()
            except Exception:
                continue
            else:
                if p.returncode == 0:
                    found = True
                    conf.env.GCC_TARGETS.append((name, bindir, cc, cxx, version, target, gcc_arch, gcc_option))
            finally:
                node.delete()
                tgtnode.delete()
        if not found:
            conf.env.GCC_TARGETS.append((name, bindir, cc, cxx, version, target, arch, []))


@conf
def detect_gcc_archs_from_path(conf, name, bindir, gcc, gxx, version, target, arch):
    v = version.split('.')
    versions = [
        '.'.join(v),
        ''.join(v),
        '.'.join(v[0:2]),
        ''.join(v[0:2]),
        v[0],
        '',
    ]

    cc = cxx = None
    for v in versions:
        cc = conf.find_program('%s-%s-%s'%(target, gcc, v), var='CC', path_list=[bindir], mandatory=False)
        if cc:
            break
        cc = conf.find_program('%s-%s%s'%(target, gcc, v), var='CC', path_list=[bindir], mandatory=False)
        if cc:
            break
    for v in versions:
        cxx = conf.find_program('%s-%s-%s'%(target, gxx, v), var='CXX', path_list=[bindir], mandatory=False)
        if cxx:
            break
        cxx = conf.find_program('%s-%s%s'%(target, gxx, v), var='CXX', path_list=[bindir], mandatory=False)
        if cxx:
            break
    del conf.env['CC']
    del conf.env['CXX']
    conf.add_all_archs_to_env(name, bindir, cc, cxx, version, target, arch)


@conf
def detect_gcc_from_path(conf, path, seen):
    for subdir, relative in [('', '../..'), ('lib/gcc', '../../../..'), ('gcc', '../../..')]:
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
                arch = conf.parse_gcc_target(target) or 'unknown'
                if libdir.find('llvm') != -1:
                    gcc, gxx = 'llvm-gcc', 'llvm-g++'
                    name = 'llvm'
                else:
                    gcc, gxx = 'gcc', 'g++'
                    name = 'gcc'
                bindir = os.path.normpath(os.path.join(libdir, relative, 'bin'))
                if (gcc, gxx, version, target, arch) not in seen:
                    conf.detect_gcc_archs_from_path(name, bindir, gcc, gxx, version, target, arch)

@conf
def get_native_gcc(conf):
    import platform
    if platform.uname()[0].lower() == 'freebsd':
        gcc = '/usr/bin/gcc'
        cmd = [gcc, '-v']
        try:
            p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE)
            out = p.communicate()[1]
        except:
            return (None, None)

        if not isinstance(out, str):
            out = out.decode(sys.stdout.encoding)
        out = out.split('\n')

        arch = None
        platform = None
        version = None

        while out:
            line = out.pop(0)
            if line.startswith('Target: '):
                target = line.split()[1].split('-')
                arch = target[0]
                platform = target[2]
            elif line.startswith('gcc version'):
                version = line.split()[2]
        if arch and platform and version:
            conf.add_all_archs_to_env('gcc', '/usr/bin/', '/usr/bin/gcc', '/usr/bin/g++', version, platform, arch)
        else:
            Logs.warn('could not find native GCC for freebsd')




@conf
def detect_gcc(conf):
    compilers_gcc_found = []
    bindirs = os.environ['PATH'].split(os.pathsep) + conf.env.EXTRA_PATH
    paths = [os.path.normpath(os.path.join(path, '..', 'lib')) for path in bindirs]
    paths = set([path for path in paths if os.path.isdir(path)])
    for bindir in bindirs:
        try:
            for f in os.listdir(os.path.join(bindir, '..')):
                subdirs = os.listdir(os.path.join(bindir, '..', f))
                if 'lib' in subdirs:
                    paths.add(os.path.normpath(os.path.join(bindir, '..', f, 'lib')))
        except OSError:
            pass
    paths = paths.union(conf.env.SYSTEM_LIB_PATHS)
    for path in paths:
        try:
            for lib in os.listdir(path):
                if lib.startswith('gcc'):
                    conf.detect_gcc_from_path(os.path.join(path, lib), compilers_gcc_found)
        except OSError:
            pass
    conf.get_native_gcc()


@conf
def load_gcc(self, bindir, gcc, gxx, version, target, arch, options):
    self.env.CC = gcc
    self.env.CXX = gxx
    self.env.COMPILER_NAME='gcc'
    self.env.COMPILER_TARGET=target
    if not self.env.AR:
        self.find_program(target + '-ar', var='AR', path_list=[bindir], mandatory=False)
    if not self.env.AR:
        self.find_program('ar', var='AR', path_list=[bindir], mandatory=False)
    if not self.env.GDB:
        self.find_program(target + '-gdb', var='GDB', path_list=[bindir], mandatory=False)
    if not self.env.GDB:
        self.find_program('gdb', var='GDB', path_list=[bindir], mandatory=False)
    self.load('gcc gxx')
    if target.find('mingw') != -1:
        winres = self.find_program(target + '-windres', var='WINRC', path_list=[bindir], mandatory=False)
        if not winres:
            winres = self.find_program('windres', var='WINRC', path_list=[bindir], mandatory=False)
        if not winres:
            winres = self.find_program('windres', var='WINRC', mandatory=False)
        self.load('winres')

    cmd = self.env.CC + options + ['-x', 'c++', '-v', '-Wp,-v', '-E', '-']
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
                self.env.SYSROOT = os.path.normpath(sysroot)
            elif line.startswith('#include <...>'):
                while out:
                    path = out.pop(0).strip()
                    if path.startswith('End of search list'):
                        break
                    self.env.append_unique('SYSTEM_INCLUDES', [os.path.normpath(path)])

    cmd = self.env.CC + options + ['-x', 'c++', '-dM', '-E', '-']
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
                    if define in self.env.useful_defines:
                        self.env.append_unique('SYSTEM_DEFINES', ['%s=%s'%(define, value)])
                else:
                    if line in self.env.useful_defines:
                        self.env.append_unique('SYSTEM_DEFINES', [line])

    cmd = self.env.CC + options + ['-x', 'c++', '-print-search-dirs']
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
                line = line[10:].strip()
                self.env.append_unique('SYSTEM_LIBPATHS', line.split(os.pathsep))

    try:
        node = self.bldnode.make_node('main.c')
        outnode = node.change_ext('')
        node.write('int main(){}\n')
        cmd = self.env.CC + options + ['-v', node.abspath(), '-o', outnode.abspath()]
        p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE)
        out, err = p.communicate()
    except Exception as e:
        print('could not retrieve system defines: %s' % str(e))
    else:
        if not isinstance(out, str):
            out = out.decode(sys.stdout.encoding)
        if not isinstance(err, str):
            err = err.decode(sys.stdout.encoding)
        out = out.split('\n') + err.split('\n')
        while out:
            line = out.pop(0)
            sysroot = line.find('--sysroot')
            if sysroot != -1:
                sysroot = shlex.split(line[sysroot+len('--sysroot')+1:].replace('\\', '\\\\'))[0]
                self.env.SYSROOT = os.path.normpath(sysroot)
    finally:
        node.delete()
        outnode.delete()




def options(opt):
    pass

def configure(conf):
    conf.start_msg('Looking for gcc compilers')
    conf.env.append_unique('useful_defines', ['__clang__', '__GNUC__', '__GNUG__'])
    archs = getattr(conf, 'gcc_architectures', [])
    conf.gcc_architectures = archs + [
        ('i686-w64', 'amd64'),
        ('i386', 'x86'),
        ('i486', 'x86'),
        ('i586', 'x86'),
        ('i686', 'x86'),
        ('arm-eabi', 'arm'),
        ('armv7', 'armv7'),
        ('armv6', 'armv6'),
        ('arm', 'arm'),
        ('mips64el', 'mips64el'),
        ('mipsel', 'mipsel'),
        ('mips', 'mips'),
        ('x86_64', 'amd64'),
        ('x86', 'x86'),
        ('amd64', 'amd64'),
        ('powerpc', 'powerpc'),
        ('mingw32', 'x86'),
        ('ppu', 'powerpc'),
        ('spu', 'spu'),
        ('aarch64', 'arm64')
    ]

    conf.env.GCC_TARGETS = []
    conf.detect_gcc()
    conf.env.GCC_TARGETS.sort(key= lambda x: (x[2], x[3], x[0]))
    conf.end_msg('done')

def build(bld):
    pass
