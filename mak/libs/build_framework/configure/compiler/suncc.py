from waflib import Utils, Logs, Configure
import os
import sys


class SunCC(Configure.ConfigurationContext.GnuCompiler):
    DEFINES = ['__SUNPRO_CC']
    NAMES = ('SunCC', )
    SUNCC_PLATFORMS = {
        '__gnu_linux__': 'linux-gnu',
        '__gnu__linux__': 'linux-gnu',
        '__sun': 'sunos',
    }
    SUNCC_ARCHS = {
        '__i386': 'x86',
        '__i386__': 'x86',
        '__x86_64__': 'amd64',
    }
    TOOLS = 'suncc suncxx'
    VECTORIZED_FLAGS = {
        'x86':
            (
                (
                    '.sse3', [
                        '-xarch=sse3', '-xarch=ssse3', '-D__MMX__=1', '-D__SSE__=1', '-D__SSE2__=1', '-D__SSE3__=1',
                        '-D__SSSE3__=1'
                    ]
                ),
                (
                    '.sse4', [
                        '-xarch=sse4_1', '-xarch=sse4_2', '-D__MMX__=1', '-DSSE__=1', '-DSSE2__=1', '-D__SSE3__=1',
                        '-D__SSSE3__=1', '-D__SSE4_1__=1', '-D__SSE4_2__=1', '-D__POPCNT__=1'
                    ]
                ),
                (
                    '.avx', [
                        '-xarch=avx', '-D__MMX__=1', '-D__SSE__=1', '-D__SSE2__=1', '-D__SSE3__=1', '-D__SSSE3__=1',
                        '-D__SSE4_1__=1', '-D__SSE4_2__=1', '-D__POPCNT__=1', '-D___AVX__=1', '-D__XSAVE__=1'
                    ]
                ),
                (
                    '.avx2', [
                        '-xarch=avx2',
                        '-D__MMX__=1',
                        '-D__SSE__=1',
                        '-D__SSE2__=1',
                        '-D__SSE3__=1',
                        '-D__SSSE3__=1',
                        '-D__SSE4_1__=1',
                        '-D__SSE4_2__=1',
                        '-D__POPCNT__=1',
                        '-D__AVX__=1',
                        '-D__XSAVE__=1',
                        '-D__AVX2__=1',
                    ]
                ),
            ),
        'amd64':
            (
                (
                    '.sse3', [
                        '-xarch=sse3', '-xarch=ssse3', '-D__MMX__=1', '-D__SSE__=1', '-D__SSE2__=1', '-D__SSE3__=1',
                        '-D__SSSE3__=1'
                    ]
                ),
                (
                    '.sse4', [
                        '-xarch=sse4_1', '-xarch=sse4_2', '-D__MMX__=1', '-D__SSE__=1', '-D__SSE2__=1', '-D__SSE3__=1',
                        '-D__SSSE3__=1', '-D__SSE4_1__=1', '-D__SSE4_2__=1', '-D__POPCNT__=1'
                    ]
                ),
                (
                    '.avx', [
                        '-xarch=avx', '-D__MMX__=1', '-D__SSE__=1', '-D__SSE2__=1', '-D__SSE3__=1', '-D__SSSE3__=1',
                        '-D__SSE4_1__=1', '-D__SSE4_2__=1', '-D__POPCNT__=1', '-D___AVX__=1', '-D__XSAVE__=1'
                    ]
                ),
                (
                    '.avx2', [
                        '-xarch=avx2',
                        '-D__MMX__=1',
                        '-D__SSE__=1',
                        '-D__SSE2__=1',
                        '-D__SSE3__=1',
                        '-D__SSSE3__=1',
                        '-D__SSE4_1__=1',
                        '-D__SSE4_2__=1',
                        '-D__POPCNT__=1',
                        '-D__AVX__=1',
                        '-D__XSAVE__=1',
                        '-D__AVX2__=1',
                    ]
                ),
            ),
    }

    def __init__(self, suncc, sunCC, extra_args={}, extra_env={}):
        Configure.ConfigurationContext.GnuCompiler.__init__(self, suncc, sunCC, extra_args, extra_env)
        if self.platform == 'linux-gnu':
            if self.version_number <= (5, 13, 0):
                self.add_flags('cxx', ['-library=Crun,stlport4'])
                self.add_flags('link', ['-library=Crun,stlport4', '-staticlib=Crun,stlport4'])
            if self.arch == 'amd64':
                if os.path.isdir('/lib/x86_64-linux-gnu'):
                    self.add_flags('link', ['-L/lib/x86_64-linux-gnu'])
                if os.path.isdir('/usr/lib/x86_64-linux-gnu'):
                    self.add_flags('link', ['-L/usr/lib/x86_64-linux-gnu'])
            elif self.arch == 'x86':
                for arch in ('i386', 'i486', 'i586', 'i686'):
                    if os.path.isdir('/lib/%s-linux-gnu' % arch):
                        self.add_flags('link', ['-L/lib/%s-linux-gnu' % arch])
                    if os.path.isdir('/usr/lib/%s-linux-gnu' % arch):
                        self.add_flags('link', ['-L/usr/lib/%s-linux-gnu' % arch])

    def get_version(self, sunCC, extra_args, extra_env):
        result, out, err = self.run([sunCC] + extra_args.get('cxx', []) + ['-xdumpmacros', '-E', '/dev/null'])
        if result != 0:
            raise Exception('could not run SunCC %s (%s)' % (sunCC, err))
        for l in out.split('\n') + err.split('\n'):
            if l.startswith('#define '):
                l = l.strip()[len('#define '):]
                sp = l.find(' ')
                if sp == -1:
                    macro = l
                    value = None
                else:
                    macro = l[:sp]
                    value = l[sp + 1:]
                if macro in self.SUNCC_PLATFORMS:
                    platform = self.SUNCC_PLATFORMS[macro]
                elif macro in self.SUNCC_ARCHS:
                    arch = self.SUNCC_ARCHS[macro]
                elif macro == '__SUNPRO_CC':
                    patch = value[-1]
                    minor = value[-3:-1]
                    major = value[2:-3]
                    version = '%s.%s%s' % (major, minor, patch if patch != '0' else '')
        self.target = '%s-%s' % (arch, platform)
        self.targets = (self.target, )
        return version, platform, arch

    def set_optimisation_options(self, conf):
        v = conf.env
        v['CPPFLAGS_debug'] = ['-D_DEBUG']
        v['CFLAGS_debug'] = ['-g', '-D_DEBUG']
        v['CXXFLAGS_debug'] = ['-g', '-D_DEBUG']
        v['LINKFLAGS_debug'] = ['-g']

        v['CPPFLAGS_profile'] = ['-DNDEBUG']
        v['CFLAGS_profile'] = ['-g', '-DNDEBUG', '-fast']
        v['CXXFLAGS_profile'] = [
            '-g', '-DNDEBUG', '-fast', '-features=mutable', '-features=localfor', '-features=bool',
            '-features=no%split_init'
        ]
        v['LINKFLAGS_profile'] = ['-g']

        v['CPPFLAGS_final'] = ['-DNDEBUG']
        v['CFLAGS_final'] = ['-g', '-DNDEBUG', '-fast']
        v['CXXFLAGS_final'] = [
            '-g', '-DNDEBUG', '-fast', '-features=mutable', '-features=localfor', '-features=bool',
            '-features=no%split_init'
        ]
        if self.version_number[0:2] != (5, 13):
            v['CXXFLAGS_profile'] += ['-features=no%except']
            v['CXXFLAGS_final'] += ['-features=no%except']
        v['LINKFLAGS_final'] = ['-g']

    def set_warning_options(self, conf):
        v = conf.env
        v['CFLAGS_warnnone'] = ['-w', '-errtags=yes', '-erroff=%all']
        v['CXXFLAGS_warnnone'] = ['-w', '-errtags=yes', '-erroff=%all']
        v['CFLAGS_warnall'] = ['+w2', '-errtags=yes']
        v['CXXFLAGS_warnall'] = [
            '+w2', '-errtags=yes', '-erroff=fieldsemicolonw,notused,'
            'unknownpragma,wunreachable,doubunder,wvarhidenmem,wvarhidemem,'
            'reftotemp,truncwarn,badargtype2w,hidef,wemptydecl,notemsource,'
            'nonewline,inllargeuse'
        ]

    def error_flag(self):
        return ['-errwarn=%all']

    def is_valid(self, conf, extra_flags=[]):
        node = conf.bldnode.make_node('main.cxx')
        tgtnode = node.change_ext('')
        node.write(
            '#ifndef _GNU_SOURCE\n# define _GNU_SOURCE\n#endif\n#include <cstdlib>\n#include <iostream>\nint main() {}\n'
        )
        try:
            result, out, err = self.run_cxx([node.abspath(), '-c', '-o', tgtnode.abspath()] + extra_flags)
        except Exception as e:
            return False
        finally:
            node.delete()
            tgtnode.delete()
        err = err.split('\n')
        if not result:
            for msg in err:
                if msg.find('illegal value ignored') != -1:
                    result = 1
        return result == 0

    def load_in_env(self, conf, platform):
        Configure.ConfigurationContext.GnuCompiler.load_in_env(self, conf, platform)
        v = conf.env
        v['RPATH_ST'] = '-R%s'
        if platform.NAME == 'Linux':
            v.IDIRAFTER = '-I'
            #v.STATIC = 1
            if self.arch == 'x86':
                v.append_unique('SYSTEM_LIBPATHS', ['=/usr/lib/i386-linux-gnu'])
                v.CFLAGS += ['-xtarget=opteron', '-I/usr/include/i386-linux-gnu']
                v.CXXFLAGS += [
                    os.path.join(conf.bugenginenode.abspath(),
                                 'mak/compiler/suncc/interlocked-a=x86.il'), '-xarch=sse2', '-xchip=generic',
                    '-xcache=64/64/2:1024/64/16', '-I/usr/include/i386-linux-gnu', '-include', 'math.h'
                ]
            elif self.arch == 'amd64':
                v.append_unique('SYSTEM_LIBPATHS', ['=/usr/lib/x86_64-linux-gnu'])
                v.CFLAGS += ['-xtarget=opteron', '-I/usr/include/x86_64-linux-gnu']
                v.CXXFLAGS += [
                    os.path.join(conf.bugenginenode.abspath(),
                                 'mak/compiler/suncc/interlocked-a=amd64.il'), '-xarch=sse2', '-xchip=generic',
                    '-xcache=64/64/2:1024/64/16', '-I/usr/include/x86_64-linux-gnu', '-include', 'math.h'
                ]
            v.append_unique('CFLAGS', ['-mt', '-xldscope=hidden', '-Kpic', '-DPIC', '-D__PIC__'])
            v.append_unique('CXXFLAGS', ['-mt', '-xldscope=hidden', '-Kpic', '-DPIC', '-D__PIC__'])
            v.append_unique('LINKFLAGS', ['-lrt', '-mt', '-znow', '-xldscope=hidden']) #, '-z', 'absexec', '-Kpic'])
            v.CFLAGS_exportall = ['-xldscope=symbolic']
            v.CXXFLAGS_exportall = ['-xldscope=symbolic']
            v.SHLIB_MARKER = '-Bdynamic'
            v.STLIB_MARKER = '-Bstatic'

    def populate_useful_variables(self, conf, sysroot):
        pass


def detect_suncc(conf):
    seen = set([])
    environ = getattr(conf, 'environ', os.environ)
    bindirs = environ['PATH'].split(os.pathsep) + conf.env.EXTRA_PATH
    paths = [os.path.normpath(os.path.join(path, '..', 'lib')) for path in bindirs]
    paths = set([path for path in paths if os.path.isdir(path)])
    for bindir in bindirs:
        suncc = conf.detect_executable('suncc', path_list=[bindir])
        sunCC = conf.detect_executable('sunCC', path_list=[bindir])
        if suncc and sunCC:
            c = SunCC(suncc, sunCC)
            if c.name() in seen:
                continue
            if not c.is_valid(conf):
                continue
            seen.add(c.name())
            conf.compilers.append(c)
            for multilib_compiler in c.get_multilib_compilers():
                if multilib_compiler.name() in seen:
                    continue
                if not multilib_compiler.is_valid(conf):
                    continue
                seen.add(multilib_compiler.name())
                conf.compilers.append(multilib_compiler)


def configure(conf):
    conf.start_msg('Looking for suncc compilers')
    detect_suncc(conf)
    conf.end_msg('done')
