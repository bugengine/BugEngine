from waflib import Configure
import os
import re


class Windows(Configure.ConfigurationContext.Platform):
    NAME = 'windows'
    SUPPORTED_TARGETS = (re.compile('.*mingw32.*'), re.compile('.*windows-gnu'), re.compile('.*windows-msvc'),
                         re.compile('.*windows-intel'), re.compile('.*windows-wsdk'))

    def is_valid(self, compiler):
        node = self.conf.bldnode.make_node('main.cxx')
        tgtnode = node.change_ext('')
        node.write('#include <cstdio>\n#include <cfloat>\nint main() {}\n')
        try:
            result, out, err = compiler.run_cxx([node.abspath(), '-x', 'c++', '-o', tgtnode.abspath()])
        except Exception as e:
            return False
        finally:
            node.delete()
            tgtnode.delete()
        return result == 0

    def __init__(self, conf=None):
        Configure.ConfigurationContext.Platform.__init__(self)
        self.conf = conf

    def get_available_compilers(self, compiler_list):
        result = []
        for c in compiler_list:
            for regexp in self.SUPPORTED_TARGETS:
                if regexp.match(c.platform):
                    if 'Clang' in c.NAMES:
                        if self.is_valid(c):
                            result.append((c, [], Windows_Clang()))
                    elif 'GCC' in c.NAMES:
                        if self.is_valid(c):
                            result.append((c, [], Windows_GCC()))
                    elif 'msvc' in c.NAMES:
                        if c.arch in ('amd64', 'x86'):
                            result.append((c, [], Windows_MSVC()))
                    else:
                        result.append((c, [], self))
        return result

    def load_in_env(self, conf, compiler):
        env = conf.env
        env.ABI = 'pe'
        env.VALID_PLATFORMS = ['windows', 'pc']
        env.pymodule_PATTERN = '%s.pyd'

        env.DEPLOY_ROOTDIR = ''
        env.DEPLOY_BINDIR = ''
        env.DEPLOY_RUNBINDIR = ''
        env.DEPLOY_LIBDIR = 'lib'
        env.DEPLOY_INCLUDEDIR = 'include'
        env.DEPLOY_DATADIR = 'data'
        env.DEPLOY_PLUGINDIR = 'data/plugin'
        env.DEPLOY_KERNELDIR = 'data/kernel'

        if compiler.arch == 'arm':
            env.MS_PROJECT_PLATFORM = 'ARM'
        elif compiler.arch == 'amd64':
            env.MS_PROJECT_PLATFORM = 'x64'
        else:
            env.MS_PROJECT_PLATFORM = 'Win32'

        env.append_unique(
            'DEFINES',
            ['_WIN32_WINNT=0x0502', 'WINVER=0x0502', '_CRT_SECURE_NO_DEPRECATE=1', '_CRT_SECURE_NO_WARNINGS=1'])

    def find_winres(self, conf, compiler):
        winres = conf.find_program(compiler.target + '-windres',
                                   var='WINRC',
                                   path_list=compiler.directories,
                                   mandatory=False)
        if not winres:
            winres = conf.find_program('windres', var='WINRC', path_list=compiler.directories, mandatory=False)
        if not winres:
            winres = conf.find_program('windres', var='WINRC', mandatory=False)
        conf.load('winres_patch', tooldir=[os.path.join(conf.bugenginenode.abspath(), 'mak', 'tools')])
        if compiler.arch == 'x86':
            conf.env.append_unique('WINRCFLAGS', ['--target=pe-i386'])
        elif compiler.arch == 'amd64':
            conf.env.append_unique('WINRCFLAGS', ['--target=pe-x86-64'])

    def platform_name(self, compiler):
        return compiler.platform_name


class Windows_Clang(Windows):
    def load_in_env(self, conf, compiler):
        Windows.load_in_env(self, conf, compiler)
        env = conf.env
        env.append_unique('LINKFLAGS', ['-static-libgcc'])
        env.append_unique('CXXFLAGS_warnall', ['-Wno-unknown-pragmas', '-Wno-comment'])
        self.find_winres(conf, compiler)
        conf.env.DEFINES_console = ['_CONSOLE=1']
        env.LINKFLAGS_console = ['-mconsole']
        env.STRIP_BINARY = True


class Windows_GCC(Windows):
    def load_in_env(self, conf, compiler):
        Windows.load_in_env(self, conf, compiler)
        env = conf.env
        env.append_unique('CFLAGS', ['-static-libgcc'])
        env.append_unique('CXXFLAGS', ['-static-libgcc'])
        env.append_unique('LINKFLAGS', ['-static-libgcc'])
        env.append_unique('CXXFLAGS_warnall', ['-Wno-unknown-pragmas', '-Wno-comment'])
        if compiler.version_number >= (4, 5):
            env.append_unique('CFLAGS', ['-static-libstdc++'])
            env.append_unique('CXXFLAGS', ['-static-libstdc++'])
            env.append_unique('LINKFLAGS', ['-static-libstdc++'])
        self.find_winres(conf, compiler)
        env.DEFINES_console = ['_CONSOLE=1']
        env.LINKFLAGS_console = ['-mconsole']
        env.STRIP_BINARY = True


class Windows_MSVC(Windows):
    def load_in_env(self, conf, compiler):
        Windows.load_in_env(self, conf, compiler)
        if compiler.arch == 'arm':
            conf.env.CPPFLAGS.append('/D_ARM_WINAPI_PARTITION_DESKTOP_SDK_AVAILABLE=1')
            conf.env.CFLAGS.append('/D_ARM_WINAPI_PARTITION_DESKTOP_SDK_AVAILABLE=1')
            conf.env.CXXFLAGS.append('/D_ARM_WINAPI_PARTITION_DESKTOP_SDK_AVAILABLE=1')
        conf.env.DEFINES_console = ['_CONSOLE=1']
        conf.env.LINKFLAGS_console = ['/SUBSYSTEM:console']


def options(opt):
    pass


def configure(conf):
    conf.platforms.append(Windows(conf))