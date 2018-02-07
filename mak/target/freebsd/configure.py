from waflib import Configure
import os
import re


class FreeBSD(Configure.ConfigurationContext.Platform):
    NAME = 'FreeBSD'
    SUPPORTED_TARGETS = (re.compile('.*-freebsd[0-9\.]*'),
                         re.compile('^freebsd[0-9\.]*'),)

    def __init__(self):
        Configure.ConfigurationContext.Platform.__init__(self)

    def load_in_env(self, conf, compiler):
        env = conf.env
        env.DEST_OS = 'freebsd'
        env.ABI = 'elf'
        env.VALID_PLATFORMS = ['freebsd', 'posix', 'pc']
        if compiler.arch.startswith('arm') and compiler.arch != 'arm64':
            if 'GCC' in compiler.NAMES:
                env.append_value('CXXFLAGS', ['-nostdinc++', '-isystem', '/usr/include/c++/v1'])
                env.LINK_CC = ['/usr/bin/cc']
                env.LINK_CXX = ['/usr/bin/c++']
            elif 'Clang' in compiler.NAMES:
                env.append_unique('CPPFLAGS', ['-mfloat-abi=hard', '-mfpu=vfp'])
                env.append_unique('CFLAGS', ['-mfloat-abi=hard', '-mfpu=vfp'])
                env.append_unique('CXXFLAGS', ['-std=gnu++11', '-mfloat-abi=hard', '-mfpu=vfp'])
                if compiler.version_number < (3,4):
                    env.append_unique('CXXFLAGS', ['-std=gnu++11'])
                if compiler.version_number < (3,5):
                    env.append_unique('CFLAGS', ['-Wa,-meabi=5'])
                    env.append_unique('CXXFLAGS', ['-Wa,-meabi=5'])
                if '-target' not in env.CFLAGS and not env.SYSROOT and env.FREEBSD_HOST_TRIPLE:
                    env.append_unique('CPPFLAGS', ['-target', env.FREEBSD_HOST_TRIPLE])
                    env.append_unique('CFLAGS', ['-target', env.FREEBSD_HOST_TRIPLE])
                    env.append_unique('CXXFLAGS', ['-target', env.FREEBSD_HOST_TRIPLE])
                    env.append_unique('LINKFLAGS', ['-target', env.FREEBSD_HOST_TRIPLE])

        env.DEPLOY_ROOTDIR = ''
        env.DEPLOY_BINDIR = 'bin'
        env.DEPLOY_RUNBINDIR = 'lib'
        env.DEPLOY_LIBDIR = 'lib'
        env.DEPLOY_INCLUDEDIR = 'include'
        env.DEPLOY_DATADIR = os.path.join('share', 'bugengine')
        env.DEPLOY_PLUGINDIR = os.path.join('share', 'bugengine', 'plugin')
        env.DEPLOY_KERNELDIR = os.path.join('share', 'bugengine', 'kernel')
        env.pymodule_PATTERN = '%s.so'
        env.STRIP_BINARY = True

        env.append_unique('CPPFLAGS', ['-fPIC'])
        env.append_unique('CFLAGS', ['-fPIC'])
        env.append_unique('CXXFLAGS', ['-fPIC'])
        env.RPATH = '$ORIGIN/../share/bugengine/plugin:$ORIGIN/../lib:$ORIGIN:$ORIGIN/../plugin:$ORIGIN/../../../lib'
        env.append_unique('LIB', ['rt', 'pthread', 'm'])
        env.append_unique('CFLAGS', ['-I%s/usr/local/include'%(compiler.sysroot or '')])
        env.append_unique('CXXFLAGS', ['-I%s/usr/local/include'%(compiler.sysroot or '')])
        env.append_unique('LIBPATH', ['%s/usr/local/lib'%(compiler.sysroot or '')])
        env.append_unique('DEFINES', ['_BSD_SOURCE'])
        env.append_unique('LINKFLAGS_dynamic', ['-rdynamic', '-Wl,-E', '-Wl,-z,origin'])


def options(opt):
    pass


def configure(conf):
    conf.platforms.append(FreeBSD())

