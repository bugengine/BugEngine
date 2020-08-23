from waflib import Configure
import os
import re


class FreeBSD(Configure.ConfigurationContext.Platform):
    NAME = 'FreeBSD'
    SUPPORTED_TARGETS = (
        re.compile('.*-freebsd[0-9\.]*'),
        re.compile('^freebsd[0-9\.]*'),
    )

    def __init__(self):
        Configure.ConfigurationContext.Platform.__init__(self)

    def get_available_compilers(self, configuration_context, compiler_list):
        result = []
        for c in compiler_list:
            for regexp in self.SUPPORTED_TARGETS:
                if regexp.match(c.platform):
                    # FreeBSD is not really multiarch, check that proper libs are installed
                    tmpnode = configuration_context.bldnode.make_node('a.out')
                    try:
                        return_code, out, err = c.run_c(
                            ['-x', 'c', '-o', tmpnode.abspath(), '-'], 'int main(int argc, char* argv[]) {  }'
                        )
                        if return_code == 0:
                            result.append((c, [], self))
                    finally:
                        try:
                            tmpnode.delete()
                        except OSError:
                            pass
        return result

    def load_in_env(self, conf, compiler):
        env = conf.env
        env.DEST_OS = 'freebsd'
        env.ABI = 'elf'
        env.COMPILER_ABI = 'freebsd'
        env.VALID_PLATFORMS = ['freebsd', 'posix', 'pc']
        sysroot = compiler.sysroot if compiler.sysroot is not None else ''
        if '-m32' in compiler.extra_args['cxx']:
            env.PKGCONFIG_DISABLE = True
            ldconfig = '%s/usr/local/libdata/ldconfig32' % sysroot
            if 'GCC' in compiler.NAMES:
                gcc_name = 'gcc' + compiler.compiler_c.split('gcc')[-1]
                gcc_libpath = os.path.join(os.path.dirname(os.path.dirname(compiler.compiler_c)), 'lib32')
                if os.path.isdir(sysroot + gcc_libpath):
                    env.append_unique('LINKFLAGS', ['-B%s%s' % (sysroot, gcc_libpath)])
                if os.path.isdir(sysroot + os.path.join(gcc_libpath, gcc_name)):
                    env.append_unique('LINKFLAGS', ['-B%s%s' % (sysroot, os.path.join(gcc_libpath, gcc_name))])
                if os.path.isdir('%s/usr/lib32' % sysroot):
                    env.append_unique('LINKFLAGS', ['-B%s/usr/lib32' % sysroot])
                if os.path.isdir('%s/usr/local/lib32' % sysroot):
                    env.append_unique('LINKFLAGS', ['-B%s/usr/local/lib32' % sysroot])
            else:
                if os.path.isdir('%s/usr/lib32' % sysroot):
                    env.append_unique('LINKFLAGS', ['-L%s/usr/lib32' % sysroot])
                if os.path.isdir('%s/usr/local/lib32' % sysroot):
                    env.append_unique('LINKFLAGS', ['-L%s/usr/local/lib32' % sysroot])
        else:
            env.append_unique('LINKFLAGS', ['-L%s/usr/local/lib' % sysroot])
            ldconfig = '%s/usr/local/libdata/ldconfig' % (compiler.sysroot or '')
        libpaths = []
        for ldconfig_conf in os.listdir(ldconfig):
            with open(os.path.join(ldconfig, ldconfig_conf), 'r') as conf_file:
                for line in conf_file:
                    if line not in libpaths:
                        libpaths.append('%s%s' % (compiler.sysroot or '', line))
        if compiler.arch.startswith('arm') and compiler.arch != 'arm64':
            if 'GCC' in compiler.NAMES:
                env.append_value('CXXFLAGS', ['-nostdinc++', '-isystem', '/usr/include/c++/v1'])
                env.LINK_CC = ['/usr/bin/cc']
                env.LINK_CXX = ['/usr/bin/c++']
            elif 'Clang' in compiler.NAMES:
                env.append_unique('CPPFLAGS', ['-mfloat-abi=hard', '-mfpu=vfp'])
                env.append_unique('CFLAGS', ['-mfloat-abi=hard', '-mfpu=vfp'])
                env.append_unique('CXXFLAGS', ['-std=gnu++11', '-mfloat-abi=hard', '-mfpu=vfp'])
                if compiler.version_number < (3, 4):
                    env.append_unique('CXXFLAGS', ['-std=gnu++11'])
                if compiler.version_number < (3, 5):
                    env.append_unique('CFLAGS', ['-Wa,-meabi=5'])
                    env.append_unique('CXXFLAGS', ['-Wa,-meabi=5'])
                if '-target' not in env.CFLAGS and not env.SYSROOT and env.FREEBSD_HOST_TRIPLE:
                    env.append_unique('CPPFLAGS', ['-target', env.FREEBSD_HOST_TRIPLE])
                    env.append_unique('CFLAGS', ['-target', env.FREEBSD_HOST_TRIPLE])
                    env.append_unique('CXXFLAGS', ['-target', env.FREEBSD_HOST_TRIPLE])
                    env.append_unique('LINKFLAGS', ['-target', env.FREEBSD_HOST_TRIPLE])
        if 'Clang' in compiler.NAMES:
            env.append_unique('CXXFLAGS_debug', ['-fdwarf-exceptions'])
            env.append_unique('CXXFLAGS', ['-stdlib=libc++'])
            env.append_unique('LINKFLAGS', ['-stdlib=libc++'])

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

        env.append_unique('CPPFLAGS', ['-fPIC'])
        env.append_unique('CFLAGS', ['-fPIC'])
        env.append_unique('CXXFLAGS', ['-fPIC'])
        if env.ARCH_LP64:
            env.RPATH = [
                ':'.join(
                    ['$ORIGIN', '$ORIGIN/../', '$ORIGIN/../lib/', '$ORIGIN/../lib/bugengine/']
                )
            ]
        else:
            env.RPATH = [
                ':'.join(
                    ['$ORIGIN', '$ORIGIN/../', '$ORIGIN/../lib32/', '$ORIGIN/../lib32/bugengine/',
                     '$ORIGIN/../lib/', '$ORIGIN/../lib/bugengine/']
                )
            ]

        env.append_unique('LIB', ['rt', 'pthread', 'm'])
        env.append_unique('CFLAGS', ['-I%s/usr/local/include' % (compiler.sysroot or '')])
        env.append_unique('CXXFLAGS', ['-I%s/usr/local/include' % (compiler.sysroot or '')])
        env.append_unique('LIBPATH', libpaths)
        env.append_unique('DEFINES', ['_BSD_SOURCE'])
        env.append_unique('LINKFLAGS_dynamic', ['-rdynamic', '-Wl,-E', '-Wl,-z,origin'])


def configure(conf):
    conf.platforms.append(FreeBSD())
