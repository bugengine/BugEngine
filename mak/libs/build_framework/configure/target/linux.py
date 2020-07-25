from waflib import Configure
import os
import re


class Linux(Configure.ConfigurationContext.Platform):
    NAME = 'Linux'
    SUPPORTED_TARGETS = (
        re.compile('.*-linux-gnu.*'),
        re.compile('^linux-gnu.*'),
        re.compile('.*-linux$'),
        re.compile('^linux$'),
    )

    def __init__(self):
        Configure.ConfigurationContext.Platform.__init__(self)

    def is_valid(self, configuration_context, compiler):
        node = configuration_context.bldnode.make_node('main.cxx')
        tgtnode = node.change_ext('')
        node.write('#include <cstdio>\n#include <cfloat>\n#include <new>\nint main() {}\n')
        try:
            result, out, err = compiler.run_cxx([node.abspath(), '-o', tgtnode.abspath()])
        except Exception as e:
            return False
        finally:
            #if result:
            #    print(compiler.name(), err)
            node.delete()
            tgtnode.delete()
        return result == 0

    def get_available_compilers(self, configuration_context, compiler_list):
        result = []
        for c in compiler_list:
            for regexp in self.SUPPORTED_TARGETS:
                if regexp.match(c.platform) and self.is_valid(configuration_context, c):
                    result.append((c, [], self))
        return result

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
        env.STRIP_BINARY = True

        if 'SunCC' not in compiler.NAMES:
            env.append_unique('CPPFLAGS', ['-fPIC'])
            env.append_unique('CFLAGS', ['-fPIC'])
            env.append_unique('CXXFLAGS', ['-fPIC'])
            env.COMPILER_ABI = 'gnu'
        else:
            env.COMPILER_ABI = 'sun'
        env.append_unique('DEFINES', ['_GNU_SOURCE'])
        env.RPATH = ['$ORIGIN/../share/bugengine/plugin:$ORIGIN/../lib:$ORIGIN:$ORIGIN/../plugin:$ORIGIN/../../../lib']
        env.append_unique('LIB', ['dl', 'rt', 'pthread', 'm'])
        env.append_unique('LINKFLAGS_dynamic', ['-Wl,--export-dynamic', '-Wl,-E', '-Wl,-z,origin'])

    def platform_name(self, compiler):
        return compiler.platform_name


def configure(conf):
    conf.platforms.append(Linux())
