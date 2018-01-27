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
