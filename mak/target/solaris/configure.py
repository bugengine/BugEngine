# set SunOS specific options

from waflib import Utils, Logs, Errors
from waflib.Configure import conf
import os, sys


def options(opt):
    pass

@conf
def set_sunos_options(self):
    self.env.ABI = 'elf'
    self.env.VALID_PLATFORMS = ['sunos', 'posix', 'pc']

    self.env.DEPLOY_ROOTDIR = ''
    self.env.DEPLOY_BINDIR = 'bin'
    self.env.DEPLOY_RUNBINDIR = 'lib'
    self.env.DEPLOY_LIBDIR = 'lib'
    self.env.DEPLOY_INCLUDEDIR = 'include'
    self.env.DEPLOY_DATADIR = os.path.join('share', 'bugengine')
    self.env.DEPLOY_PLUGINDIR = os.path.join('share', 'bugengine', 'plugin')
    self.env.DEPLOY_KERNELDIR = os.path.join('share', 'bugengine', 'kernel')
    self.env.pymodule_PATTERN = '%s.so'

    self.env.RPATH = '$ORIGIN/../plugin'
    for i in os.environ['PATH'].split(':'):
        self.env.append_unique('RPATH', [os.path.join(os.path.dirname(i), 'lib')])
    self.env.append_unique('LIB', ['dl', 'rt', 'pthread', 'm'])

@conf
def set_sunos_suncc_options(conf, arch):
    v = conf.env
    if arch in ['x86', 'i386', 'i486', 'i586', 'i686']:
        v.append_unique('CFLAGS', ['-m32'])
        v.append_unique('CXXFLAGS', ['-m32', os.path.join(conf.bugenginenode.abspath(), 'mak/compiler/suncc/interlocked-a=x86.il')])
        v.append_unique('LINKFLAGS', ['-m32'])
    elif arch in ['x86_64', 'x64', 'amd64']:
        v.append_unique('CFLAGS', ['-m64'])
        v.append_unique('CXXFLAGS', ['-m64', os.path.join(conf.bugenginenode.abspath(), 'mak/compiler/suncc/interlocked-a=amd64.il')])
        v.append_unique('LINKFLAGS', ['-m64'])
    v.append_unique('CFLAGS', ['-mt', '-xldscope=hidden', '-Kpic', '-DPIC', '-D__PIC__'])
    v.append_unique('CXXFLAGS', ['-mt', '-xldscope=hidden', '-Kpic', '-DPIC', '-D__PIC__'])
    v.append_unique('LINKFLAGS', ['-lposix4', '-mt', '-znow', '-xldscope=hidden', '-Kpic', '-ldl', '-lm'])

    v['RPATH_ST'] = '-R%s'

    v['CFLAGS_warnnone'] = ['-w', '-errtags=yes', '-erroff=%all']
    v['CXXFLAGS_warnnone'] = ['-w', '-errtags=yes', '-erroff=%all']
    v['CFLAGS_warnall'] = ['-errtags=yes']
    v['CXXFLAGS_warnall'] = ['-errtags=yes', '-erroff=fieldsemicolonw,notused,unknownpragma,wunreachable,doubunder,wvarhidenmem,wvarhidemem,reftotemp,truncwarn,badargtype2w,hidef,wemptydecl,notemsource,nonewline,inllargeuse']

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

@conf
def set_sunos_gcc_options(conf, flags, version):
    v = conf.env
    version_number = conf.get_gcc_version_float(version)
    v.append_unique('CFLAGS', flags + ['-fpic', '-fno-jump-tables'])
    v.append_unique('CXXFLAGS', flags + ['-fpic', '-fno-jump-tables'])
    v.append_unique('LINKFLAGS', flags + ['-static-libgcc', '-lposix4', '-lstdc++'])

    v.CFLAGS_warnnone = ['-w']
    v.CXXFLAGS_warnnone = ['-w']
    v.CFLAGS_warnall = ['-std=c99', '-Wall', '-Wextra', '-pedantic', '-Winline', '-Werror']
    v.CXXFLAGS_warnall = ['-Wall', '-Wextra', '-Werror', '-Wno-sign-compare', '-Woverloaded-virtual', '-Wno-invalid-offsetof']
    if version_number >= 4.8:
        v.CXXFLAGS_warnall.append('-Wno-unused-local-typedefs')

    v.CFLAGS_debug = ['-pipe', '-g', '-D_DEBUG']
    v.CXXFLAGS_debug = ['-g', '-D_DEBUG']
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

@conf
def set_sunos_clang_options(conf, version):
    v = conf.env
    version = version.split('.')
    version = float(version[0]) + float(version[1])/10
    v.CFLAGS = options + ['-fPIC']
    v.CXXFLAGS = options + ['-fPIC']
    v.LINKFLAGS = options + ['-rdynamic', '-Wl,-E']
    v.CXXFLAGS_cxxshlib = []

    v.CFLAGS_warnnone = ['-w']
    v.CXXFLAGS_warnnone = ['-w']
    v.CFLAGS_warnall = ['-std=c99', '-Wall', '-Wextra', '-pedantic', '-Winline', '-Werror']
    v.CXXFLAGS_warnall = ['-Wall', '-Wextra', '-Werror', '-Wno-sign-compare', '-Woverloaded-virtual', '-Wno-invalid-offsetof']

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


def configure(conf):
    seen = set([])
    for name, bindir, gcc, gxx, version, target, arch, options in conf.env.GCC_TARGETS:
        position = target.find('solaris')
        if position != -1:
            os = 'sunos'
            toolchain = '%s-%s-%s-%s'%(os, arch, name, version)
            if toolchain not in seen:
                seen.add(toolchain)
                env = conf.env.derive()
                conf.setenv(toolchain, env)
                env.DEST_OS = 'sunos'
                try:
                    conf.start_msg('Setting up compiler')
                    conf.load_gcc(bindir, gcc, gxx, version, target, arch, options)
                    conf.set_sunos_gcc_options(options, version)
                except Exception as e:
                    conf.end_msg(e, color='RED')
                    raise
                else:
                    conf.end_msg('done')
                    conf.set_sunos_options()
                    try:
                        conf.env.KERNEL_TOOLCHAINS = [toolchain]
                        conf.add_toolchain(os, arch, name, version, arch)
                    except Errors.WafError as e:
                        conf.variant = ''
                        Logs.pprint('YELLOW', '%s failed: %s' % (toolchain, e))
                    except Exception as e:
                        conf.variant = ''
                        Logs.pprint('RED', '%s failed: %s' % (toolchain, e))
                        raise
                    else:
                        conf.variant = ''
                        Logs.pprint('GREEN', 'configured for toolchain %s' % (toolchain))

    for version, directory, target, arch in conf.env.CLANG_TARGETS:
        if target.find('solaris') != -1:
            arch_name, options = arch
            target = 'sunos'
            toolchain = '%s-%s-%s-%s'%(target, arch_name, 'clang', version)
            if toolchain not in seen:
                seen.add(toolchain)
                env = conf.env.derive()
                conf.setenv(toolchain, env)
                env.DEST_OS = 'sunos'
                try:
                    conf.start_msg('Setting up compiler')
                    conf.load_clang(directory, target, options)
                    conf.set_sunos_clang_options(options, version)
                except Exception as e:
                    conf.end_msg(e, color='RED')
                    raise
                else:
                    conf.end_msg('done')
                    try:
                        conf.set_sunos_options()
                        conf.env.KERNEL_TOOLCHAINS = [toolchain]
                        conf.add_toolchain(target, arch_name, 'clang', version, arch_name)
                    except Errors.WafError as e:
                        conf.variant = ''
                        Logs.pprint('YELLOW', '%s failed: %s' % (toolchain, e))
                    except Exception as e:
                        conf.variant = ''
                        Logs.pprint('RED', '%s failed: %s' % (toolchain, e))
                        raise
                    else:
                        conf.variant = ''
                        Logs.pprint('GREEN', 'configured for toolchain %s' % (toolchain))


    for directory, target, arch, version in conf.env.SUNCC_TARGETS:
        if target.find('SunOS') != -1:
            target = 'sunos'
            toolchain = '%s-%s-%s-%s'%(target, arch, 'suncc', version)
            if toolchain not in seen:
                seen.add(toolchain)
                env = conf.env.derive()
                conf.setenv(toolchain, env)
                env.DEST_OS = 'sunos'
                try:
                    conf.start_msg('Setting up compiler')
                    conf.load_suncc(directory)
                    conf.set_sunos_suncc_options(arch)
                except Exception as e:
                    conf.end_msg(e, color='RED')
                    raise
                else:
                    conf.end_msg('done')
                    try:
                        conf.set_sunos_options()
                        conf.env.KERNEL_TOOLCHAINS = [toolchain]
                        conf.add_toolchain(target, arch, 'suncc', version, arch)
                    except Errors.WafError as e:
                        conf.variant = ''
                        Logs.pprint('YELLOW', '%s failed: %s' % (toolchain, e))
                    except Exception as e:
                        conf.variant = ''
                        Logs.pprint('RED', '%s failed: %s' % (toolchain, e))
                        raise
                    else:
                        conf.variant = ''
                        Logs.pprint('GREEN', 'configured for toolchain %s' % (toolchain))

