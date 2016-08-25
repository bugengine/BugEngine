from mak import targets
from mak.compiler import clang, gcc
import re


class Windows(targets.Platform):
    NAME = 'windows'
    SUPPORTED_TARGETS = (re.compile('.*mingw32.*'),
                         re.compile('.*windows-gnu'),
                         re.compile('.*windows-msvc'),
                         re.compile('.*windows-icl'),
                         re.compile('.*windows-wsdk'))

    def __init__(self):
        super(Windows, self).__init__()

    def get_available_compilers(self, compiler_list):
        result = []
        for c in compiler_list:
            for regexp in self.SUPPORTED_TARGETS:
                if regexp.match(c.platform):
                    if 'Clang' in c.NAMES:
                        result.append((c, [], Windows_Clang()))
                    elif 'GCC' in c.NAMES:
                        result.append((c, [], Windows_GCC()))
                    elif 'msvc' in c.NAMES:
                        result.append((c, [], Windows_MSVC()))
                    else:
                        result.append((c, [], self))
        return result

    def load_in_env(self, conf, compiler):
        env = conf.env
        env.ABI = 'pe'
        env.VALID_PLATFORMS = ['windows', 'pc']
        env.pymodule_PATTERN = '%s.pyd'

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

        env.append_unique('DEFINES', ['_WIN32_WINNT=0x0502', 'WINVER=0x0502'])

    def find_winres(self, conf, compiler):
        winres = conf.find_program(compiler.target + '-windres', var='WINRC',
                                   path_list=compiler.directories, mandatory=False)
        if not winres:
            winres = conf.find_program('windres', var='WINRC', path_list=compiler.directories,
                                       mandatory=False)
        if not winres:
            winres = conf.find_program('windres', var='WINRC', mandatory=False)
        conf.load('winres')


class Windows_Clang(Windows):
    def load_in_env(self, conf, compiler):
        super(Windows_Clang, self).load_in_env(conf, compiler)
        env = conf.env
        env.append_unique('LINKFLAGS', ['-static-libgcc', '-static-libstdc++', '-pthread'])
        env.append_unique('STLIB', ['pthread'])
        env.append_unique('CXXFLAGS_warnall', ['-Wno-unknown-pragmas', '-Wno-comment'])
        self.find_winres(conf, compiler)
        env.CFLAGS_console = ['-D_CONSOLE=1']
        env.CXXFLAGS_console = ['-D_CONSOLE=1']
        env.LINKFLAGS_console = ['-mconsole']


class Windows_GCC(Windows):
    def load_in_env(self, conf, compiler):
        super(Windows_GCC, self).load_in_env(conf, compiler)
        env = conf.env
        env.append_unique('CFLAGS', ['-static-libgcc'])
        env.append_unique('CXXFLAGS', ['-static-libgcc'])
        env.append_unique('LINKFLAGS', ['-static-libgcc'])
        env.append_unique('CXXFLAGS_warnall', ['-Wno-unknown-pragmas', '-Wno-comment'])
        if compiler.version_number >= 4.5:
            env.append_unique('CFLAGS', ['-static-libstdc++'])
            env.append_unique('CXXFLAGS', ['-static-libstdc++'])
            env.append_unique('LINKFLAGS', ['-static-libstdc++'])
        self.find_winres(conf, compiler)
        env.CFLAGS_console = ['-D_CONSOLE=1']
        env.CXXFLAGS_console = ['-D_CONSOLE=1']
        env.LINKFLAGS_console = ['-mconsole']


class Windows_MSVC(Windows):
    def load_in_env(self, conf, compiler):
        super(Windows_MSVC, self).load_in_env(conf, compiler)
        if compiler.arch == 'arm':
            conf.env.CFLAGS.append('/D_ARM_WINAPI_PARTITION_DESKTOP_SDK_AVAILABLE=1')
            conf.env.CXXFLAGS.append('/D_ARM_WINAPI_PARTITION_DESKTOP_SDK_AVAILABLE=1')
        conf.env.CFLAGS_console = ['/D_CONSOLE=1']
        conf.env.CXXFLAGS_console = ['/D_CONSOLE=1']
        conf.env.LINKFLAGS_console = ['/SUBSYSTEM:console']


def options(opt):
    pass


def configure(conf):
    conf.platforms.append(Windows())


def build(bld):
    bld.platforms.append(bld.external('3rdparty.win32'))
    bld.platforms.append(bld.external('3rdparty.dbghelp'))


def plugins(bld):
    pass
