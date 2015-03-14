# set BSD specific options

import os
from waflib import Context, Errors, Utils
from waflib.Logs import info,warn,pprint
from waflib.TaskGen import feature, before_method, after_method
from waflib.Configure import conf


@conf
def set_freebsd_options(self):
    self.env.ABI = 'elf'
    self.env.VALID_PLATFORMS = ['freebsd', 'posix', 'pc']

    self.env.DEPLOY_ROOTDIR = ''
    self.env.DEPLOY_BINDIR = 'bin'
    self.env.DEPLOY_RUNBINDIR = 'lib'
    self.env.DEPLOY_LIBDIR = 'lib'
    self.env.DEPLOY_INCLUDEDIR = 'include'
    self.env.DEPLOY_DATADIR = os.path.join('share', 'bugengine')
    self.env.DEPLOY_PLUGINDIR = os.path.join('share', 'bugengine', 'plugin')
    self.env.DEPLOY_KERNELDIR = os.path.join('share', 'bugengine', 'kernel')
    self.env.pymodule_PATTERN = '%s.so'

    self.env.RPATH = '$ORIGIN/../share/bugengine/plugin:$ORIGIN/../lib:$ORIGIN:$ORIGIN/../plugin'
    self.env.append_unique('LIB', ['rt', 'pthread', 'm'])
    self.env.append_unique('INCLUDES', ['/usr/local/include'])
    self.env.append_unique('LIBPATH', ['/usr/local/lib'])

@conf
def set_freebsd_gcc_options(self, flags, version):
    v = self.env
    version_number = self.get_gcc_version_float(version)
    if version_number >= 4:
        v.CFLAGS = flags + ['-fPIC', '-fvisibility=hidden']
        v.CXXFLAGS = flags + ['-fPIC', '-fvisibility=hidden']
        v.CFLAGS_exportall = ['-fvisibility=default']
        v.CXXFLAGS_exportall = ['-fvisibility=default']
    else:
        v.CFLAGS = flags + ['-fPIC']
        v.CXXFLAGS = flags + ['-fPIC']
    v.LINKFLAGS = flags + ['-rdynamic', '-Wl,-E', '-Wl,-z,origin']
    v.CXXFLAGS_cxxshlib = []

    v.CFLAGS_warnnone = ['-w']
    v.CXXFLAGS_warnnone = ['-w']
    v.CFLAGS_warnall = ['-std=c99', '-Wall', '-Wextra', '-pedantic', '-Winline', '-Werror']
    v.CXXFLAGS_warnall = ['-Wall', '-Wextra', '-Werror', '-Wno-sign-compare', '-Woverloaded-virtual', '-Wno-invalid-offsetof']
    if version_number >= 4.8:
        v.CXXFLAGS_warnall.append('-Wno-unused-local-typedefs')

    v.CFLAGS_debug = ['-pipe', '-g', '-D_DEBUG']
    v.CXXFLAGS_debug = ['-pipe', '-g', '-D_DEBUG']
    v.ASFLAGS_debug = ['-pipe', '-g', '-D_DEBUG']
    v.LINKFLAGS_debug = ['-pipe']

    v.CFLAGS_profile = ['-pipe', '-g', '-DNDEBUG', '-O3']
    v.CXXFLAGS_profile = ['-pipe', '-Wno-unused-parameter', '-g', '-DNDEBUG', '-O3', '-fno-rtti', '-fno-exceptions']
    v.ASFLAGS_profile = ['-pipe', '-g', '-DNDEBUG', '-O3']
    v.LINKFLAGS_profile = ['-pipe']

    v.CFLAGS_final = ['-pipe', '-g', '-DNDEBUG', '-O3']
    v.CXXFLAGS_final = ['-pipe', '-Wno-unused-parameter', '-g', '-DNDEBUG', '-O3', '-fno-rtti', '-fno-exceptions']
    v.ASFLAGS_final = ['-pipe', '-g', '-DNDEBUG', '-O3']
    v.LINKFLAGS_final = ['-pipe']


@conf
def set_freebsd_clang_options(self, options, version):
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
    v.LINKFLAGS = options + ['-rdynamic', '-Wl,-E', '-Wl,-z,origin']
    v.CXXFLAGS_cxxshlib = []

    v.CFLAGS_warnnone = ['-w']
    v.CXXFLAGS_warnnone = ['-w']
    v.CFLAGS_warnall = ['-std=c99', '-Wall', '-Wextra', '-pedantic', '-Winline', '-Werror']
    v.CXXFLAGS_warnall = ['-Wall', '-Wextra', '-Werror', '-Wno-sign-compare', '-Woverloaded-virtual', '-Wno-invalid-offsetof']

    v.CFLAGS_debug = ['-pipe', '-g', '-D_DEBUG']
    v.CXXFLAGS_debug = ['-pipe', '-g', '-D_DEBUG']
    v.ASFLAGS_debug = ['-pipe', '-g', '-D_DEBUG']
    v.LINKFLAGS_debug = ['-pipe']

    v.CFLAGS_profile = ['-pipe', '-g', '-DNDEBUG', '-O3']
    v.CXXFLAGS_profile = ['-pipe', '-Wno-unused-parameter', '-g', '-DNDEBUG', '-O3', '-fno-rtti', '-fno-exceptions']
    v.ASFLAGS_profile = ['-pipe', '-g', '-DNDEBUG', '-O3']
    v.LINKFLAGS_profile = ['-pipe']

    v.CFLAGS_final = ['-pipe', '-g', '-DNDEBUG', '-O3']
    v.CXXFLAGS_final = ['-pipe', '-Wno-unused-parameter', '-g', '-DNDEBUG', '-O3', '-fno-rtti', '-fno-exceptions']
    v.ASFLAGS_final = ['-pipe', '-g', '-DNDEBUG', '-O3']
    v.LINKFLAGS_final = ['-pipe']

def options(opt):
    pass

def configure(conf):
    seen = set([])
    for name, bindir, gcc, gxx, version, target, arch, options in conf.env.GCC_TARGETS:
        position = target.find('freebsd')
        if position != -1:
            os = 'freebsd'
            toolchain = '%s-%s-%s-%s'%(os, arch, name, version)
            if toolchain not in seen:
                seen.add(toolchain)
                env = conf.env.derive()
                conf.setenv(toolchain, env)
                env.DEST_OS = 'freebsd'
                try:
                    conf.start_msg('Setting up compiler')
                    conf.load_gcc(bindir, gcc, gxx, version, target, arch, options)
                    conf.set_freebsd_gcc_options(options, version)
                except Exception as e:
                    conf.end_msg(e, color='RED')
                    raise
                else:
                    conf.end_msg('done')
                    try:
                        conf.set_freebsd_options()
                        conf.env.KERNEL_TOOLCHAINS = [toolchain]
                        conf.add_toolchain(os, arch, name, version, arch)
                    except Errors.WafError as e:
                        conf.variant = ''
                        pprint('YELLOW', '%s failed: %s' % (toolchain, e))
                    except Exception as e:
                        conf.variant = ''
                        pprint('RED', '%s failed: %s' % (toolchain, e))
                        raise
                    else:
                        conf.variant = ''
                        pprint('GREEN', 'configured for toolchain %s' % (toolchain))

    for version, directory, target, arch in conf.env.CLANG_TARGETS:
        if target.find('freebsd') != -1:
            arch_name, options = arch
            target = 'freebsd'
            toolchain = '%s-%s-%s-%s'%(target, arch_name, 'clang', version)
            if toolchain not in seen:
                seen.add(toolchain)
                env = conf.env.derive()
                conf.setenv(toolchain, env)
                env.DEST_OS = 'freebsd'
                try:
                    conf.start_msg('Setting up compiler')
                    conf.load_clang(directory, target, options)
                    conf.set_freebsd_clang_options(options, version)
                except Exception as e:
                    conf.end_msg(e, color='RED')
                    raise
                else:
                    conf.end_msg('done')
                    try:
                        conf.set_freebsd_options()
                        conf.env.KERNEL_TOOLCHAINS = [toolchain]
                        conf.add_toolchain(target, arch_name, 'clang', version, arch_name)
                    except Errors.WafError as e:
                        conf.variant = ''
                        pprint('YELLOW', '%s failed: %s' % (toolchain, e))
                    except Exception as e:
                        conf.variant = ''
                        pprint('RED', '%s failed: %s' % (toolchain, e))
                        raise
                    else:
                        conf.variant = ''
                        pprint('GREEN', 'configured for toolchain %s' % (toolchain))

def build(bld):
    bld.platforms.append(bld.external('3rdparty.X11'))

def plugins(bld):
    pass
