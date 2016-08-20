from waflib import Utils, Logs
from waflib.Configure import conf
from mak import compilers
import os, sys



class SunCC(compilers.GnuCompiler):
    DEFINES = ['__SUNPRO_CC']
    NAMES = ('SunCC',)
    SUNCC_PLATFORMS = {
        '__gnu_linux__':    'linux-gnu',
        '__gnu__linux__':   'linux-gnu',
        '__sun':            'sunos',
    }
    SUNCC_ARCHS = {
        '__i386__': 'x86',
        '__x86_64__': 'amd64',
    }
    TOOLS = 'suncc suncxx'

    def __init__(self, suncc, sunCC, extra_args = [], extra_env={}):
        super(SunCC, self).__init__(suncc, sunCC, extra_args, extra_env)
        if self.platform == 'linux-gnu':
            self.extra_args.append('-library=Crun,stlport4')
            if self.arch == 'amd64':
                if os.path.isdir('/usr/lib/x86_64-linux-gnu'):
                    self.extra_args.append('-L/usr/lib/x86_64-linux-gnu')
            elif self.arch == 'x86':
                if os.path.isdir('/usr/lib/i386-linux-gnu'):
                    self.extra_args.append('-L/usr/lib/i386-linux-gnu')
                if os.path.isdir('/usr/lib/i486-linux-gnu'):
                    self.extra_args.append('-L/usr/lib/i486-linux-gnu')
                if os.path.isdir('/usr/lib/i586-linux-gnu'):
                    self.extra_args.append('-L/usr/lib/i586-linux-gnu')
                if os.path.isdir('/usr/lib/i686-linux-gnu'):
                    self.extra_args.append('-L/usr/lib/i686-linux-gnu')

    def get_version(self, sunCC, extra_args, extra_env):
        result, out, err = self.run([sunCC] + extra_args + ['-xdumpmacros', '-E', '/dev/null'])
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
                    value = l[sp+1:]
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
        return version, platform, arch

    def set_optimisation_options(self, conf):
        v = conf.env
        v['CFLAGS_debug'] = ['-g', '-D_DEBUG']
        v['CXXFLAGS_debug'] = ['-g', '-D_DEBUG']
        v['LINKFLAGS_debug'] = ['-g']

        v['CFLAGS_profile'] = ['-g', '-DNDEBUG', '-fast']
        v['CXXFLAGS_profile'] = ['-g', '-DNDEBUG', '-fast',
                                '-features=no%except', '-features=mutable',
                                '-features=localfor', '-features=bool', '-features=no%split_init']
        v['LINKFLAGS_profile'] = ['-g']

        v['CFLAGS_final'] = ['-g', '-DNDEBUG', '-fast']
        v['CXXFLAGS_final'] = ['-g', '-DNDEBUG', '-fast',
                                '-features=no%except', '-features=mutable',
                                '-features=localfor', '-features=bool', '-features=no%split_init']
        v['LINKFLAGS_final'] = ['-g']

    def set_warning_options(self, conf):
        v = conf.env
        v['RPATH_ST'] = '-R%s'

        v['CFLAGS_warnnone'] = ['-w', '-errtags=yes', '-erroff=%all']
        v['CXXFLAGS_warnnone'] = ['-w', '-errtags=yes', '-erroff=%all']
        v['CFLAGS_warnall'] = ['+w2', '-errtags=yes']
        v['CXXFLAGS_warnall'] = ['+w2', '-errtags=yes', '-erroff=fieldsemicolonw,notused,'
                                 'unknownpragma,wunreachable,doubunder,wvarhidenmem,wvarhidemem,'
                                 'reftotemp,truncwarn,badargtype2w,hidef,wemptydecl,notemsource,'
                                 'nonewline,inllargeuse']


    def load_in_env(self, conf, platform):
        super(SunCC, self).load_in_env(conf, platform)
        v = conf.env
        if platform.NAME == 'Linux':
            v.STATIC = 1
            if self.arch == 'x86':
                v.append_unique('SYSTEM_LIBPATHS', ['=/usr/lib/i386-linux-gnu'])
                v.append_unique('CFLAGS', ['-xarch=sse2', '-I/usr/include/i386-linux-gnu'])
                v.append_unique('CXXFLAGS', [os.path.join(conf.bugenginenode.abspath(),
                                                          'mak/compiler/suncc/interlocked-a=x86.il'),
                                             '-xarch=sse2', '-I/usr/include/i386-linux-gnu',
                                             '-include', 'cstdio'])
                v.append_unique('LINKFLAGS', [])
            elif self.arch == 'amd64':
                v.append_unique('SYSTEM_LIBPATHS', ['=/usr/lib/x86_64-linux-gnu'])
                v.append_unique('CFLAGS', ['-I/usr/include/x86_64-linux-gnu'])
                v.append_unique('CXXFLAGS', [os.path.join(conf.bugenginenode.abspath(),
                                                          'mak/compiler/suncc/interlocked-a=amd64.il'),
                                             '-I/usr/include/x86_64-linux-gnu',
                                             '-include', 'cstdio'])
                v.append_unique('LINKFLAGS', [])
            v.append_unique('CFLAGS', ['-mt', '-xldscope=hidden', '-Kpic', '-DPIC', '-D__PIC__'])
            v.append_unique('CXXFLAGS', ['-mt', '-xldscope=hidden', '-Kpic', '-DPIC', '-D__PIC__', '-library=Crun,stlport4'])
            v.append_unique('LINKFLAGS', ['-lrt', '-mt', '-znow', '-xldscope=hidden', '-z', 'absexec', '-Kpic', '-library=Crun,stlport4', '-staticlib=Crun,stlport4'])

    def populate_useful_variables(self, conf):
        pass

def detect_suncc(conf):
    seen = set([])
    bindirs = os.environ['PATH'].split(os.pathsep) + conf.env.EXTRA_PATH
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


def options(opt):
    pass

def configure(conf):
    conf.start_msg('Looking for suncc compilers')
    detect_suncc(conf)
    conf.end_msg('done')

def build(bld):
    pass

