# set Linux specific options

import os
from waflib import Context, Errors, Utils
from waflib.Logs import info,warn,pprint
from waflib.TaskGen import feature, before_method, after_method
from waflib.Configure import conf


@conf
def set_linux_options(self):
    self.env.ABI = 'elf'
    self.env.VALID_PLATFORMS = ['linux', 'posix', 'pc']

    self.env.DEPLOY_ROOTDIR = ''
    self.env.DEPLOY_BINDIR = 'bin'
    self.env.DEPLOY_RUNBINDIR = 'lib'
    self.env.DEPLOY_LIBDIR = 'lib'
    self.env.DEPLOY_INCLUDEDIR = 'include'
    self.env.DEPLOY_DATADIR = os.path.join('share', 'bugengine')
    self.env.DEPLOY_PLUGINDIR = os.path.join('share', 'bugengine', 'plugin')
    self.env.DEPLOY_KERNELDIR = os.path.join('share', 'bugengine', 'kernel')

    self.env.LIBPATH += self.env.SYSTEM_LIB_PATHS

    self.env.RPATH = '$ORIGIN/../share/bugengine/plugin:$ORIGIN/../lib:$ORIGIN:$ORIGIN/../plugin'
    self.env.append_unique('LIB', ['dl', 'rt', 'pthread', 'm'])

@conf
def set_linux_suncc_options(conf, arch):
    v = conf.env
    v.STATIC = True
    if arch in ['x86', 'i386', 'i486', 'i586', 'i686']:
        v.append_unique('CFLAGS', ['-m32', '-xarch=sse2', '-I/usr/include/i386-linux-gnu'])
        v.append_unique('CXXFLAGS', ['-m32', os.path.join(conf.bugenginenode.abspath(), 'mak/compiler/suncc/interlocked-a=x86.il'), '-xarch=sse2', '-I/usr/include/i386-linux-gnu'])
        v.append_unique('LINKFLAGS', ['-m32'])
    elif arch in ['x86_64', 'x64', 'amd64']:
        v.append_unique('CFLAGS', ['-m64', '-I/usr/include/x86_64-linux-gnu'])
        v.append_unique('CXXFLAGS', ['-m64', os.path.join(conf.bugenginenode.abspath(), 'mak/compiler/suncc/interlocked-a=amd64.il'), '-I/usr/include/x86_64-linux-gnu'])
        v.append_unique('LINKFLAGS', ['-m64'])
    v.append_unique('CFLAGS', ['-mt', '-xldscope=hidden', '-Kpic', '-DPIC', '-D__PIC__'])
    v.append_unique('CXXFLAGS', ['-mt', '-xldscope=hidden', '-Kpic', '-DPIC', '-D__PIC__', '-library=Crun,stlport4'])
    v.append_unique('LINKFLAGS', ['-lrt', '-mt', '-znow', '-xldscope=hidden', '-z absexec', '-Kpic', '-library=Crun,stlport4', '-staticlib=%all'])

    v['RPATH_ST'] = '-R%s'

    v['CFLAGS_warnnone'] = ['-w', '-errtags=yes', '-erroff=%all']
    v['CXXFLAGS_warnnone'] = ['-w', '-errtags=yes', '-erroff=%all']
    v['CFLAGS_warnall'] = ['+w2', '-errtags=yes']
    v['CXXFLAGS_warnall'] = ['+w2', '-errtags=yes', '-erroff=fieldsemicolonw,notused,unknownpragma,wunreachable,doubunder,wvarhidenmem,wvarhidemem,reftotemp,truncwarn,badargtype2w,hidef,wemptydecl,notemsource,nonewline,inllargeuse']

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
def set_linux_icc_options(self, flags, version, arch):
    v = self.env
    version = float(version)
    if version >= 11:
        v.CFLAGS = flags + ['-fPIC', '-fvisibility=hidden']
        v.CXXFLAGS = flags + ['-fPIC', '-fvisibility=hidden']
        v.CFLAGS_exportall = ['-fvisibility=default']
        v.CXXFLAGS_exportall = ['-fvisibility=default']
    else:
        v.CFLAGS = flags + ['-fPIC']
        v.CXXFLAGS = flags + ['-fPIC']
    v.LINKFLAGS = flags + ['-rdynamic', '-Wl,-E', '-static-libgcc', '-static-intel']

    v.CFLAGS_warnnone = ['-w']
    v.CXXFLAGS_warnnone = ['-w']
    v.CFLAGS_warnall = ['-std=c99', '-Wall', '-Wextra', '-pedantic', '-Winline', '-Werror']
    v.CXXFLAGS_warnall = ['-Wall', '-Wextra', '-Werror', '-Wno-sign-compare', '-Woverloaded-virtual', '-Wno-invalid-offsetof', '-wd597']

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



@conf
def set_linux_gcc_options(self, flags, version):
    v = self.env
    version = version.split('.')
    version = float(version[0]) + float(version[1])/10
    if version >= 4:
        v.CFLAGS = flags + ['-fPIC', '-fvisibility=hidden']
        v.CXXFLAGS = flags + ['-fPIC', '-fvisibility=hidden']
        v.CFLAGS_exportall = ['-fvisibility=default']
        v.CXXFLAGS_exportall = ['-fvisibility=default']
    else:
        v.CFLAGS = flags + ['-fPIC']
        v.CXXFLAGS = flags + ['-fPIC']
    v.LINKFLAGS = flags + ['-rdynamic', '-Wl,-E']

    v.CFLAGS_warnnone = ['-w']
    v.CXXFLAGS_warnnone = ['-w']
    v.CFLAGS_warnall = ['-std=c99', '-Wall', '-Wextra', '-pedantic', '-Winline', '-Werror']
    v.CXXFLAGS_warnall = ['-Wall', '-Wextra', '-Werror', '-Wno-sign-compare', '-Woverloaded-virtual', '-Wno-invalid-offsetof']
    if version >= 4.8:
        v.CXXFLAGS_warnall.append('-Wno-unused-local-typedefs')

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


@conf
def set_linux_clang_options(self, options, version):
    v = self.env
    version = version.split('.')
    version = float(version[0]) + float(version[1])/10
    if version >= 3.1:
        v.CFLAGS = options + ['-fPIC', '-fvisibility=hidden']
        v.CXXFLAGS = options + ['-fPIC', '-fvisibility=hidden']
        v.CFLAGS_exportall = ['-fvisibility=default']
        v.CXXFLAGS_exportall = ['-fvisibility=default']
    else:
        v.CFLAGS = options + ['-fPIC']
        v.CXXFLAGS = options + ['-fPIC']
    v.LINKFLAGS = options + ['-rdynamic', '-Wl,-E']

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

def options(opt):
    pass

def configure(conf):
    seen = set([])
    for directory, target, arch, version in conf.env.SUNCC_TARGETS:
        if target == 'Linux':
            target = 'linux'
            toolchain = '%s-%s-%s-%s'%(target, arch, 'suncc', version)
            if toolchain not in seen:
                seen.add(toolchain)
                env = conf.env.derive()
                conf.setenv(toolchain, env)
                env.DEST_OS = 'linux'
                try:
                    conf.start_msg('Setting up compiler')
                    conf.load_suncc(directory)
                    conf.set_linux_suncc_options(arch)
                except Exception as e:
                    conf.end_msg(e, color='RED')
                else:
                    conf.end_msg('done')
                    try:
                        conf.set_linux_options()
                        conf.env.KERNEL_TOOLCHAINS = [toolchain]
                        conf.add_toolchain(target, arch, 'suncc', version, arch)
                    except Errors.WafError as e:
                        conf.variant = ''
                        pprint('YELLOW', '%s failed: %s' % (toolchain, e))
                    except Exception as e:
                        conf.variant = ''
                        raise
                    else:
                        conf.variant = ''
                        pprint('GREEN', 'configured for toolchain %s' % (toolchain))

    for name, bindir, gcc, gxx, version, target, arch, options in conf.env.ICC_TARGETS:
        position = target.find('linux')
        if position != -1:
            os_name = 'linux'
            toolchain = '%s-%s-%s-%s'%(os_name, arch, name, version)
            if toolchain not in seen:
                seen.add(toolchain)
                env = conf.env.derive()
                conf.setenv(toolchain, env)
                env.DEST_OS = 'linux'
                try:
                    conf.start_msg('Setting up compiler')
                    conf.load_icc(bindir, gcc, gxx, version, target, arch, options)
                    conf.set_linux_icc_options(options, version, arch)
                except Exception as e:
                    conf.end_msg(e, color='RED')
                else:
                    conf.end_msg('done')
                    try:
                        conf.set_linux_options()
                        conf.env.KERNEL_TOOLCHAINS = [toolchain]
                        conf.add_toolchain(os_name, arch, name, version, arch)
                    except Errors.WafError as e:
                        conf.variant = ''
                        pprint('YELLOW', '%s failed: %s' % (toolchain, e))
                    except Exception as e:
                        conf.variant = ''
                        raise
                    else:
                        conf.variant = ''
                        pprint('GREEN', 'configured for toolchain %s' % (toolchain))

    for name, bindir, gcc, gxx, version, target, arch, options in conf.env.GCC_TARGETS:
        position = target.find('linux-gnu')
        if position != -1:
            os_name = 'linux'
            toolchain = '%s-%s-%s-%s'%(os_name, arch, name, version)
            if toolchain not in seen:
                seen.add(toolchain)
                env = conf.env.derive()
                conf.setenv(toolchain, env)
                env.DEST_OS = 'linux'
                try:
                    conf.start_msg('Setting up compiler')
                    conf.load_gcc(bindir, gcc, gxx, version, target, arch, options)
                    conf.set_linux_gcc_options(options, version)
                except Exception as e:
                    conf.end_msg(e, color='RED')
                else:
                    try:
                        conf.end_msg('done')
                        conf.set_linux_options()
                        conf.env.KERNEL_TOOLCHAINS = [toolchain]
                        conf.add_toolchain(os_name, arch, name, version, arch)
                    except Errors.WafError as e:
                        conf.variant = ''
                        pprint('YELLOW', '%s failed: %s' % (toolchain, e))
                    except Exception as e:
                        conf.variant = ''
                        raise
                    else:
                        conf.variant = ''
                        pprint('GREEN', 'configured for toolchain %s' % (toolchain))

    for version, directory, target, arch in conf.env.CLANG_TARGETS:
        if target.find('linux') != -1:
            arch_name, options = arch
            os_name = 'linux'
            toolchain = '%s-%s-%s-%s'%(os_name, arch_name, 'clang', version)
            if toolchain in seen:
                continue
            if not conf.is_clang_working(os.path.join(directory, 'clang'), options):
                continue
            seen.add(toolchain)
            env = conf.env.derive()
            conf.setenv(toolchain, env)
            env.DEST_OS = 'linux'
            try:
                conf.start_msg('Setting up compiler')
                conf.load_clang(directory, target, options)
                conf.set_linux_clang_options(options, version)
            except Exception as e:
                conf.end_msg(e, color='RED')
            else:
                try:
                    conf.end_msg('done')
                    conf.set_linux_options()
                    conf.env.KERNEL_TOOLCHAINS = [toolchain]
                    conf.add_toolchain(os_name, arch_name, 'clang', version, arch_name)
                except Errors.WafError as e:
                    conf.variant = ''
                    pprint('YELLOW', '%s failed: %s' % (toolchain, e))
                except Exception as e:
                    conf.variant = ''
                    raise
                else:
                    conf.variant = ''
                    pprint('GREEN', 'configured for toolchain %s' % (toolchain))

def build(bld):
    bld.platforms.append(bld.external('3rdparty.X11'))

def plugins(bld):
    pass
