from waflib import Configure
import os
import re


class Solaris(Configure.ConfigurationContext.Platform):
    NAME = 'Solaris'
    SUPPORTED_TARGETS = (
        re.compile('^solaris.*'),
        re.compile('^sun-solaris.*'),
        re.compile('^sunos.*'),
    )

    def __init__(self):
        Configure.ConfigurationContext.Platform.__init__(self)

    def get_suncc_system_libpath(self, configuration_context, compiler):
        node = configuration_context.bldnode.make_node('main.cxx')
        tgtnode = node.change_ext('')
        node.write('int main() {}\n')
        try:
            result, out, err = compiler.run_cxx([node.abspath(), '-o', tgtnode.abspath(), '-###'])
        except Exception as e:
            print(e)
            pass
        finally:
            #if result:
            #    print(compiler.name(), err)
            node.delete()
            out = out.split() + err.split()
            for line in out:
                if line[0] == '"':
                    line = line[1:-1]
                if line[0:2] == 'P,':
                    configuration_context.env.append_value('SYSTEM_LIBPATHS', line[2:].split(':'))


    def is_valid(self, configuration_context, compiler, options):
        node = configuration_context.bldnode.make_node('main.cxx')
        tgtnode = node.change_ext('')
        node.write('#include <cstdio>\n#include <cfloat>\n#include <new>\nint main() {}\n')
        try:
            result, out, err = compiler.run_cxx([node.abspath(), '-o', tgtnode.abspath()] + options)
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
                if regexp.match(c.platform):
                    if self.is_valid(configuration_context, c, ['-std=c++98'] if 'Clang' in c.NAMES else []):
                        result.append((c, [], self))
                        break
            else:
                print(c.platform)
        return result

    def load_in_env(self, conf, compiler):
        env = conf.env

        env.DEST_OS = 'solaris'
        env.ABI = 'elf'
        env.VALID_PLATFORMS = ['solaris', 'sunos', 'posix', 'pc']

        env.DEPLOY_ROOTDIR = ''
        env.DEPLOY_BINDIR = 'bin'
        env.DEPLOY_RUNBINDIR = 'lib'
        env.DEPLOY_LIBDIR = 'lib'
        env.DEPLOY_INCLUDEDIR = 'include'
        env.DEPLOY_DATADIR = os.path.join('share', 'bugengine')
        env.DEPLOY_PLUGINDIR = os.path.join(env.DEPLOY_RUNBINDIR, 'bugengine')
        env.DEPLOY_KERNELDIR = os.path.join(env.DEPLOY_RUNBINDIR, 'bugengine')
        env.pymodule_PATTERN = '%s.so'
        env.STRIP_BINARY = True

        if 'SunCC' not in compiler.NAMES:
            env.append_unique('CPPFLAGS', ['-fPIC'])
            env.append_unique('CFLAGS', ['-fPIC'])
            env.append_unique('CXXFLAGS', ['-fPIC'])
            env.COMPILER_ABI = 'gnu'
            if 'Clang' in compiler.NAMES:
                env.append_unique('CXXFLAGS', ['-std=c++98'])
        else:
            env.COMPILER_ABI = 'sun'
            env.append_value('DEFINES', ['__unix__=1'])
            self.get_suncc_system_libpath(conf, compiler)
        env.append_unique('DEFINES', ['_GNU_SOURCE'])
        env.RPATH = [
            ':'.join(
                ['$ORIGIN', '$ORIGIN/../lib/', '$ORIGIN/../lib/bugengine/'] +
                ['$ORIGIN/../lib/%s' % target for target in compiler.targets] +
                ['$ORIGIN/../lib/%s/bugengine' % target for target in compiler.targets]
            )
        ]
        env.append_unique('LDFLAGS', ['-ldl', '-lrt', '-lpthread', '-lm', '-lc'])
        #env.append_unique('LINKFLAGS', ['-rdynamic'])

    def platform_name(self, compiler):
        return 'solaris'


def configure(conf):
    conf.platforms.append(Solaris())

