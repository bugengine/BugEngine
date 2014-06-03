from waflib import Errors, Logs
from waflib.Configure import conf
from waflib.TaskGen import feature, before_method, after_method
import os


@conf
def set_windows_options(self, arch):
    self.env.ABI = 'pe'
    self.env.VALID_PLATFORMS = ['windows', 'pc']
    self.env.LINK_WITH_PROGRAM = True

    self.env.DEPLOY_BINDIR = ''
    self.env.DEPLOY_RUNBINDIR = ''
    self.env.DEPLOY_LIBDIR = 'lib'
    self.env.DEPLOY_INCLUDEDIR = 'include'
    self.env.DEPLOY_DATADIR = 'data'
    self.env.DEPLOY_PLUGINDIR = 'data/plugin'
    self.env.DEPLOY_KERNELDIR = 'data/kernel'

    if arch == 'arm':
        self.env.MS_PROJECT_PLATFORM = 'ARM'
    elif arch in ['x64', 'x86_64', 'amd64']:
        self.env.MS_PROJECT_PLATFORM = 'x64'
    else:
        self.env.MS_PROJECT_PLATFORM = 'Win32'

    self.env.append_unique('DEFINES', ['_WIN32_WINNT=0x0502', 'WINVER=0x0502'])

@conf
def set_windows_gcc_options(self, options, version):
    v = self.env
    version = version.split('.')
    version = float(version[0]) + float(version[1])/10
    v.append_unique('CFLAGS', options)
    v.append_unique('CXXFLAGS', options)
    v.append_unique('LINKFLAGS', options)
    v.CFLAGS_warnnone = ['-w']
    v.CXXFLAGS_warnnone = ['-w']
    v.CFLAGS_warnall = ['-std=c99', '-Wall', '-Wextra', '-pedantic', '-Winline', '-Werror']
    v.CXXFLAGS_warnall = ['-Wall', '-Wextra', '-Werror', '-Wno-sign-compare', '-Woverloaded-virtual', '-Wno-invalid-offsetof', '-Wno-unknown-pragmas', '-Wno-comment']
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
def set_windows_clang_options(self, options, version):
    v = self.env
    v.append_unique('CFLAGS', options + ['-mwindows'])
    v.append_unique('CXXFLAGS', options + ['-mwindows'])
    v.append_unique('LINKFLAGS', options + ['-mwindows', '-Wl,--export-all-symbols'])
    v.CFLAGS_warnnone = ['-w']
    v.CXXFLAGS_warnnone = ['-w']
    v.CFLAGS_warnall = ['-std=c99', '-Wall', '-Wextra', '-pedantic', '-Winline', '-Werror']
    v.CXXFLAGS_warnall = ['-Wall', '-Wextra', '-Werror', '-Wno-sign-compare', '-Woverloaded-virtual', '-Wno-invalid-offsetof', '-Wno-unknown-pragmas', '-Wno-comment']

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
def set_windows_msvc_options(self):
    self.env.CFLAGS.append('/D_ARM_WINAPI_PARTITION_DESKTOP_SDK_AVAILABLE=1')
    self.env.CXXFLAGS.append('/D_ARM_WINAPI_PARTITION_DESKTOP_SDK_AVAILABLE=1')
    self.env.CFLAGS_warnall = ['/D_CRT_SECURE_NO_WARNINGS=1', '/W4', '/WX']
    self.env.CFLAGS_warnnone = ['/D_CRT_SECURE_NO_WARNINGS=1', '/W0']
    self.env.CXXFLAGS_warnall = ['/D_CRT_SECURE_NO_WARNINGS=1', '/W4', '/WX']
    self.env.CXXFLAGS_warnnone = ['/D_CRT_SECURE_NO_WARNINGS=1', '/W0']

    self.env.CFLAGS_debug = ['/Od', '/Ob1', '/EHsc', '/RTC1', '/RTCc', '/Zi', '/MTd', '/D_DEBUG']
    self.env.CXXFLAGS_debug = ['/Od', '/Ob1', '/EHsc', '/RTC1', '/RTCc', '/Zi', '/MTd', '/D_DEBUG', '/GR']
    self.env.LINKFLAGS_debug = ['/DEBUG', '/INCREMENTAL:no']
    self.env.ARFLAGS_debug = []

    self.env.CFLAGS_profile = ['/DNDEBUG', '/MT', '/Ox', '/Ob2', '/Oi', '/Ot', '/Oy', '/GT', '/GL', '/GF', '/FD', '/GS-', '/Gy', '/GR-']
    self.env.CXXFLAGS_profile = ['/DNDEBUG', '/D_HAS_EXCEPTIONS=0', '/MT', '/Ox', '/Ob2', '/Oi', '/Ot', '/Oy', '/GT', '/GL', '/GF', '/FD', '/GS-', '/Gy', '/GR-']
    self.env.LINKFLAGS_profile = ['/DEBUG', '/LTCG', '/INCREMENTAL:no']
    self.env.ARFLAGS_profile = ['/LTCG']

    self.env.CFLAGS_final = ['/DNDEBUG', '/MT', '/Ox', '/Ob2', '/Oi', '/Ot', '/Oy', '/GT', '/GL', '/GF', '/FD', '/GS-', '/Gy', '/GR-']
    self.env.CXXFLAGS_final = ['/DNDEBUG', '/D_HAS_EXCEPTIONS=0', '/MT', '/Ox', '/Ob2', '/Oi', '/Ot', '/Oy', '/GT', '/GL', '/GF', '/FD', '/GS-', '/Gy', '/GR-']
    self.env.LINKFLAGS_final = ['/DEBUG', '/LTCG', '/INCREMENTAL:no']
    self.env.ARFLAGS_final = ['/LTCG']

def options(opt):
    pass

def configure(conf):
    seen = set([])
    for version, config in conf.env.MSVC_INSTALLED_VERSIONS:
        version_name, version_number = version.split()
        if version_name in ['intel', 'wsdk', 'msvc']:
            for target_arch, infos in config:
                if (target_arch, version) in seen:
                    continue
                seen.add((target_arch, version))
                arch, (bindir, libdir, includedir) = infos
                toolchain = '%s-%s-%s-%s'%('windows', target_arch, version_name, version_number)
                env = conf.env.derive()
                conf.setenv(toolchain, env)
                env.DEST_OS = 'win32'
                try:
                    conf.start_msg('Setting up compiler')
                    conf.load_msvc(version, target_arch)
                    conf.set_windows_msvc_options()
                except Exception as e:
                    conf.variant = ''
                    conf.end_msg(e, color='RED')
                else:
                    conf.end_msg('done')
                    try:
                        conf.set_windows_options(arch)
                        conf.env.KERNEL_TOOLCHAINS = [toolchain]
                        conf.add_toolchain('windows', target_arch, version_name, version_number, arch)
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


    for name, bindir, gcc, gxx, version, target, arch, options in conf.env.GCC_TARGETS:
        position = target.find('mingw')
        if position != -1:
            os = 'windows'
            toolchain = '%s-%s-%s-%s'%(os, arch, name, version)
            if toolchain not in seen:
                seen.add(toolchain)
                env = conf.env.derive()
                conf.setenv(toolchain, env)
                env.DEST_OS = 'win32'
                try:
                    conf.start_msg('Setting up compiler')
                    conf.load_gcc(bindir, gcc, gxx, version, target, arch, options)
                    conf.set_windows_gcc_options(options, version)
                except Exception as e:
                    conf.variant = ''
                    conf.end_msg(e, color='RED')
                else:
                    conf.end_msg('done')
                    try:
                        conf.set_windows_options(arch)
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
        if target.find('win32') != -1 or target.find('mingw') != -1:
            arch_name, options = arch
            os = 'windows'
            toolchain = '%s-%s-%s-%s'%(os, arch_name, 'clang', version)
            if toolchain not in seen:
                seen.add(toolchain)
                env = conf.env.derive()
                conf.setenv(toolchain, env)
                env.DEST_OS = 'win32'
                try:
                    conf.start_msg('Setting up compiler')
                    conf.load_clang(directory, target, options)
                    conf.set_windows_clang_options(options, version)
                except Exception as e:
                    conf.variant = ''
                    conf.end_msg(e, color='RED')
                else:
                    conf.end_msg('done')
                    try:
                        conf.set_windows_options(arch)
                        conf.env.KERNEL_TOOLCHAINS = [toolchain]
                        conf.add_toolchain(os, arch_name, 'clang', version, arch_name)
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



def build(bld):
    bld.platforms.append(bld.external('3rdparty.win32'))
    bld.platforms.append(bld.external('3rdparty.dbghelp'))

def plugins(bld):
    pass


@feature('cprogram', 'cxxprogram')
@after_method('apply_link')
@before_method('apply_implib')
def apply_implib_program(self):
    if self.env.DEST_OS == 'win32':
        self.apply_implib()
