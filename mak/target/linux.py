from waflib import Configure
import os
import re


class Linux(Configure.ConfigurationContext.Platform):
    NAME = 'Linux'
    SUPPORTED_TARGETS = (re.compile('.*-linux-gnu.*'),
                         re.compile('^linux-gnu.*'),
                         re.compile('.*-linux$'),
                         re.compile('^linux$'),)

    def __init__(self):
        Configure.ConfigurationContext.Platform.__init__(self)

    def load_in_env(self, conf, compiler):
        env = conf.env
        env.DEST_OS = 'linux'
        env.ABI = 'elf'
        env.VALID_PLATFORMS = ['linux', 'posix', 'pc']

        env.DEPLOY_ROOTDIR = ''
        env.DEPLOY_BINDIR = 'bin'
        env.DEPLOY_RUNBINDIR = 'lib'
        env.DEPLOY_LIBDIR = 'lib'
        env.DEPLOY_INCLUDEDIR = 'include'
        env.DEPLOY_DATADIR = os.path.join('share', 'bugengine')
        env.DEPLOY_PLUGINDIR = os.path.join('share', 'bugengine', 'plugin')
        env.DEPLOY_KERNELDIR = os.path.join('share', 'bugengine', 'kernel')
        env.pymodule_PATTERN = '%s.so'

        if 'SunCC' not in compiler.NAMES:
            env.append_unique('CFLAGS', ['-fPIC'])
            env.append_unique('CXXFLAGS', ['-fPIC'])
        env.append_unique('DEFINES', ['_GNU_SOURCE'])
        env.RPATH = '$ORIGIN/../share/bugengine/plugin:$ORIGIN/../lib:$ORIGIN:$ORIGIN/../plugin:$ORIGIN/../../../lib'
        env.append_unique('LIB', ['dl', 'rt', 'pthread', 'm'])
        env.append_unique('LINKFLAGS_dynamic', ['-Wl,--export-dynamic', '-Wl,-E', '-Wl,-z,origin'])


def options(opt):
    pass


def configure(conf):
    conf.platforms.append(Linux())


def build(bld):
    pass


def plugins(bld):
    pass

