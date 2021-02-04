#! /usr/bin/env python
# set Android specific options

import os
import re
from copy import deepcopy
from waflib import Context, Errors, Logs, Options, Configure


def _get_android_arch(arch):
    archs = {
        'armv7a': 'arm',
        'amd64': 'x86_64',
        'mipsel': 'mips',
        'mips64el': 'mips64',
        'i686': 'x86',
        'aarch64': 'arm64'
    }
    return archs.get(arch, arch)


class NdkConfig:
    def __init__(self, ndkroot, sysroot, ldsysroot, libpath, defines):
        self._ndkroot = ndkroot
        self._sysroot = sysroot
        self._ldsysroot = ldsysroot
        self._libpath = libpath
        self._defines = defines

    def get_defines(self):
        return self._defines

    def get_ndkroot(self):
        return self._ndkroot

    def get_sysroot(self):
        return self._sysroot

    def get_ldsysroot(self):
        return self._ldsysroot

    def get_libpath(self):
        return self._libpath


class NdkArchConfig:
    def __init__(self, archs):
        self._archs = archs

    def get_ndk_config(self, arch):
        return self._archs[_get_android_arch(arch)]

    def get_valid_archs(self, archs):
        result = []
        for arch in archs:
            if _get_android_arch(arch) in self._archs:
                result.append(arch)
        return result


class NdkVersionConfig:
    def __init__(self, ndkroot):
        self._versions = {}
        platforms_directory = os.path.join(ndkroot, 'platforms')
        if os.path.isdir(platforms_directory):
            sysroot_dir = os.path.join(ndkroot, 'sysroot')
            unified_headers = os.path.isdir(sysroot_dir)
            for version in os.listdir(platforms_directory):
                if os.path.isdir(os.path.join(platforms_directory, version)):
                    defines = []
                    version_number = int(version.split('-')[1])
                    if unified_headers:
                        defines.append('-D__ANDROID_API__=%d' % version_number)
                    arch_configs = {}
                    for arch in os.listdir(os.path.join(platforms_directory, version)):
                        sysroot_arch_dir = os.path.join(platforms_directory, version, arch)
                        if os.path.isdir(sysroot_arch_dir):
                            arch_name = arch.split('-')[1]
                            if os.path.isdir(os.path.join(sysroot_arch_dir, 'usr', 'lib64')):
                                libdir = os.path.join(sysroot_arch_dir, 'usr', 'lib64')
                            else:
                                libdir = os.path.join(sysroot_arch_dir, 'usr', 'lib')
                            arch_configs[arch_name] = NdkConfig(
                                ndkroot, sysroot_dir if unified_headers else sysroot_arch_dir, sysroot_arch_dir,
                                [libdir], defines
                            )
                    self._versions[version_number] = NdkArchConfig(arch_configs)
        else:
            for toolchain in os.listdir(os.path.join(ndkroot, 'toolchains', 'llvm', 'prebuilt')):
                sysroot_dir = os.path.join(ndkroot, 'toolchains', 'llvm', 'prebuilt', toolchain, 'sysroot')
                if os.path.isdir(sysroot_dir):
                    for target in os.listdir(os.path.join(sysroot_dir, 'usr', 'lib')):
                        arch = _get_android_arch(target.split('-')[0])
                        lib_dir = os.path.join(sysroot_dir, 'usr', 'lib', target)
                        for version in os.listdir(lib_dir):
                            lib_dir_version = os.path.join(lib_dir, version)
                            if os.path.isdir(lib_dir_version):
                                config = NdkConfig(
                                    ndkroot, sysroot_dir, sysroot_dir, [lib_dir_version, lib_dir],
                                    ['-D__ANDROID_API__=%s' % version]
                                )
                                try:
                                    self._versions[int(version)]._archs[arch] = config
                                except KeyError:
                                    self._versions[int(version)] = NdkArchConfig({arch: config})

    def get_ndk_for_sdk(self, sdk):
        sdk_number = int(sdk.split('-')[1])
        ndk_versions = sorted([v for v in self._versions.keys() if v <= sdk_number])
        try:
            best_ndk_version = ndk_versions[-1]
        except IndexError:
            return None
        else:
            return self._versions[best_ndk_version]


class AndroidPlatform(Configure.ConfigurationContext.Platform):
    NAME = 'android'

    def __init__(self, conf, ndk_config, sdk_root, version):
        Configure.ConfigurationContext.Platform.__init__(self)
        self.NAME = self.__class__.NAME + '_' + self.get_android_version(version)
        self.ndk_config = ndk_config
        self.sdk_path = sdk_root
        self.sdk_version = version

    def get_android_version(self, sdk_version):
        versions = {
            '1': '1.0',
            '2': '1.1',
            '3': 'Cupcake_1.5',
            '4': 'Donut_1.6',
            '5': 'Eclair_2.0',
            '6': 'Eclair_2.0.1',
            '7': 'Eclair_2.1',
            '8': 'Froyo_2.2',
            '9': 'Gingerbread_2.3.2',
            '10': 'Gingerbread_2.3.7',
            '11': 'Honeycomb_3.0',
            '12': 'Honeycomb_3.1',
            '13': 'Honeycomb_3.2',
            '14': 'IceCreamSandwich_4.0',
            '15': 'IceCreamSandwich_4.0.3',
            '16': 'JellyBean_4.1',
            '17': 'JellyBean_4.2',
            '18': 'JellyBean_4.3',
            '19': 'KitKat_4.4',
            '20': 'KitKat_4.4W',
            '21': 'Lollipop_5.0',
            '22': 'Lollipop_5.1',
            '23': 'Marshmallow_6.0',
            '24': 'Nougat_7.0',
            '25': 'Nougat_7.1',
            '26': 'Oreo_8.0',
            '27': 'Oreo_8.1',
            '28': 'Pie_9.0',
            '29': '10',
            '30': '11',
        }
        return versions.get(sdk_version, 'api' + sdk_version)

    def get_target_folder(self, arch):
        archs = {
            'mipsel': 'mips',
            'x86': 'x86',
            'armv7a': 'armeabi-v7a',
            'arm64': 'arm64-v8a',
            'mips64el': 'mips64',
            'amd64': 'x86_64'
        }
        return archs[arch]

    def get_android_c_flags(self, compiler):
        arch_flags = {
            'gcc':
                {
                    'x86': [],
                    'amd64': [],
                    'armv7a': ['-march=armv7-a', '-mfloat-abi=softfp', '-mfpu=vfpv3-d16'],
                    'arm64': [],
                    'mipsel': [],
                    'mips64el': [],
                },
            'clang':
                {
                    'x86': [],
                    'amd64': [],
                    'armv7a': ['-fno-integrated-as', '-march=armv7-a', '-mfloat-abi=softfp', '-mfpu=vfpv3-d16'],
                    'arm64': [],
                    'mipsel': ['-fintegrated-as'],
                    'mips64el': ['-fintegrated-as'],
                }
        }
        return arch_flags[compiler.NAMES[0].lower()][compiler.arch]

    def get_android_ld_flags(self, compiler):
        arch_flags = {
            'gcc':
                {
                    'x86': [],
                    'amd64': [],
                    'armv7a': ['-Wl,--fix-cortex-a8', ],
                    'arm64': [],
                    'mipsel': [],
                    'mips64el': [],
                },
            'clang':
                {
                    'x86': [],
                    'amd64': [],
                    'armv7a': ['-Wl,--fix-cortex-a8', ],
                    'arm64': [],
                    'mipsel': [],
                    'mips64el': [],
                }
        }
        return arch_flags[compiler.NAMES[0].lower()][compiler.arch]

    def load_in_env(self, conf, compiler):
        env = conf.env
        arch = compiler.arch
        ndk_config = self.ndk_config.get_ndk_config(arch)
        target_folder = self.get_target_folder(arch)

        env.VALID_PLATFORMS = ['android']
        appname = getattr(Context.g_module, Context.APPNAME, conf.srcnode.name)
        env.cxxprogram_PATTERN = 'lib%s.so'
        env.append_unique('CFLAGS', ['-fPIC'])
        env.append_unique('CXXFLAGS', ['-fPIC'])
        env.append_unique('LINKFLAGS_cprogram', ['-shared', '-Wl,-z,defs', '-llog', '-lc'])
        env.append_unique('LINKFLAGS_cxxprogram', ['-shared', '-Wl,-z,defs', '-llog', '-lc'])
        env.LINK_WITH_PROGRAM = True
        env.STRIP_BINARY = True
        env.CFLAGS_cxxshlib = []
        env.CXXFLAGS_cxxshlib = []
        env.STATIC = True
        env.COMPILER_ABI = 'androideabi'

        env.DEPLOY_ROOTDIR = appname
        env.DEPLOY_BINDIR = os.path.join('lib', target_folder)
        env.DEPLOY_RUNBINDIR = os.path.join('lib', target_folder)
        env.DEPLOY_LIBDIR = os.path.join('lib', target_folder)
        env.DEPLOY_INCLUDEDIR = 'include'
        env.DEPLOY_DATADIR = os.path.join('assets')
        env.DEPLOY_PLUGINDIR = os.path.join('lib', target_folder)
        env.DEPLOY_KERNELDIR = os.path.join('lib', target_folder)

        env.append_value('CFLAGS', self.get_android_c_flags(compiler))
        env.append_value('CXXFLAGS', self.get_android_c_flags(compiler) + ['-nostdinc++', '-std=c++98'])
        env.append_value('LDFLAGS', self.get_android_ld_flags(compiler))

        env.ANDROID_SDK = self.sdk_version
        env.ANDROID_SDK_PATH = self.sdk_path
        env.ANDROID_NDK_PATH = ndk_config.get_ndkroot()
        env.ANDROID_ARCH = _get_android_arch(arch)
        conf.env.SYSROOT = ndk_config.get_sysroot()
        compiler.sysroot = ndk_config.get_sysroot()

        sysroot_options = ndk_config.get_defines() + [
            '-isystem',
            os.path.join(compiler.sysroot, 'usr', 'include'), '-isystem',
            os.path.join(compiler.sysroot, 'usr', 'include', compiler.target)
        ]
        env.append_unique('JAVACFLAGS', ['-bootclasspath', os.path.join(self.sdk_path, 'android.jar')])
        env.append_unique('AAPTFLAGS', ['-I', os.path.join(self.sdk_path, 'android.jar')])

        if not os.path.isfile(
            os.path.
            join(ndk_config.get_ndkroot(), 'prebuilt', 'android-%s' % env.ANDROID_ARCH, 'gdbserver', 'gdbserver')
        ):
            raise Errors.WafError('could not find gdbserver for architecture %s' % env.ANDROID_ARCH)

        conf.env.append_value('CFLAGS', sysroot_options)
        conf.env.append_value('CXXFLAGS', sysroot_options)
        conf.env.append_value(
            'LINKFLAGS', ['--sysroot', ndk_config.get_ldsysroot(),
                          '-B%s' % ndk_config.get_libpath()[0]] + ['-L%s' % l for l in ndk_config.get_libpath()]
        )


class AndroidLoader(Configure.ConfigurationContext.Platform):
    NAME = 'android'

    def __init__(self, conf):
        self.conf = conf
        Configure.ConfigurationContext.Platform.__init__(self)

        if Options.options.android_jdk:
            paths = [
                os.path.join(Options.options.android_jdk, 'bin'),
                os.path.join(Options.options.android_jdk, 'jre', 'bin')
            ]
            conf.find_program('javac', path_list=paths)
            conf.find_program('java', path_list=paths)
            conf.find_program('jar', path_list=paths)
            conf.find_program('javadoc', path_list=paths)
        conf.load('javaw')
        conf.env.append_value('JAVACFLAGS', ['-source', '1.6', '-target', '1.6'])
        key_debug = conf.path.parent.make_node('debug.keystore')
        conf.env.JARSIGNER_FLAGS = [
            '-sigalg', 'MD5withRSA', '-digestalg', 'SHA1', '-keystore',
            key_debug.abspath(), '-storepass', 'android', '-keypass', 'android'
        ]
        conf.env.JARSIGNER_KEY = 'androiddebugkey'
        conf.env.APKSIGNER_FLAGS = [
            '--ks', key_debug.abspath(), '--ks-pass', 'pass:android', '--key-pass', 'pass:android'
        ]

        sdk_build_tool_path = self.get_build_tool_path(Options.options.android_sdk_path)
        sdk_tools_paths = self.get_tools_paths(Options.options.android_sdk_path)
        conf.find_program('adb', path_list=sdk_tools_paths)
        conf.env.DEX = os.path.join(sdk_build_tool_path, 'lib', 'dx.jar')
        if not os.path.isfile(conf.env.DEX):
            raise Errors.WafError('Unable to locate dx.jar')
        conf.find_program('zipalign', var='ZIPALIGN', path_list=sdk_tools_paths + [sdk_build_tool_path])
        conf.find_program('jarsigner', var='JARSIGNER', mandatory=False)
        conf.find_program('apksigner', var='APKSIGNER', path_list=[sdk_build_tool_path], mandatory=False)
        if not conf.env.JARSIGNER and not conf.env.APKSIGNER:
            raise Errors.WafError('Unable to locate jarsigner or apksigner')
        conf.env.DEXCREATE = '--dex'
        conf.env.DEX_TGT_PATTERN = '--output=%s'
        conf.find_program('aapt', path_list=[sdk_build_tool_path])
        conf.find_program('7z', var='_7Z', mandatory=False)

    def get_tools_paths(self, android_path):
        return [os.path.join(android_path, 'platform-tools'), os.path.join(android_path, 'tools')]

    def get_build_tool_path(self, android_path):
        sdk_tools_path = os.path.join(android_path, 'build-tools')
        if os.path.isdir(sdk_tools_path):
            sdk_tools = sorted(os.listdir(sdk_tools_path))
            if sdk_tools:
                sdk_tool = sdk_tools[-1]
                return os.path.join(sdk_tools_path, sdk_tool)
        raise Errors.WafError('Android build-tools not installed')

    def find_android_sdk(self, ndk_path, sdk_path, archs):
        def alphanum_key(s):
            def tryint(s):
                try:
                    return int(s)
                except Exception:
                    return s

            return [tryint(c) for c in re.split('([0-9]+)', s)]

        def valid_archs(platform_ndk, platform, archs):
            result = []
            for arch in archs:
                a = _get_android_arch(arch)
                p = os.path.join(platform_ndk, platform, 'arch-%s' % a)
                if os.path.isdir(p):
                    result.append(arch)
            return result

        ndk_version_config = NdkVersionConfig(ndk_path)

        all_sdk_sdks = []
        platforms_sdk = os.path.join(sdk_path, 'platforms')
        all_sdk_sdks = [p for p in os.listdir(platforms_sdk)]
        sdk_pairs = [(i, ndk_version_config.get_ndk_for_sdk(i)) for i in all_sdk_sdks]
        sdk_pairs = [(i, j) for i, j in sdk_pairs if j]
        sdk_pairs = sorted(sdk_pairs, key=lambda x: alphanum_key(x[0]))
        if sdk_pairs:
            prefered_sdk = Options.options.android_sdk
            if prefered_sdk == 'all':
                return [
                    (ndk, os.path.join(platforms_sdk, sdk), ndk.get_valid_archs(archs), sdk.split('-')[1])
                    for sdk, ndk in sdk_pairs
                ]
            elif prefered_sdk:
                if 'android-%s' % prefered_sdk in all_sdk_sdks:
                    sdk = 'android-%s' % prefered_sdk
                    ndk = dict(sdk_pairs)[sdk]
                else:
                    Logs.warn(
                        'could not find android SDK version %s in path %s; using %s' % (prefered_sdk, sdk_path, sdk)
                    )
            else:
                sdk, ndk = sdk_pairs[0]
                return [(ndk, os.path.join(platforms_sdk, sdk), ndk.get_valid_archs(archs), sdk.split('-')[1])]
        else:
            raise Errors.WafError('no SDK for archs')

    def get_available_compilers(self, configuration_context, compiler_list):
        result = []
        compiler_sets = {}
        for compiler in compiler_list:
            for c in [compiler] + compiler.siblings:
                compiler_path = os.path.normpath(c.compiler_c)
                for ndk_path in Options.options.android_ndk_path.split(','):
                    ndk_path = os.path.normpath(os.path.abspath(ndk_path))
                    if compiler_path.startswith(ndk_path):
                        c_name = c.NAMES[0].lower()
                        try:
                            subset = compiler_sets[c_name]
                        except KeyError:
                            subset = compiler_sets[c_name] = {}
                        k = (c.NAMES[0], c.version, ndk_path)
                        try:
                            subset[k].append(c)
                        except KeyError:
                            subset[k] = [c]
                        break

        def add_compiler_set(compilers):
            archs = [c.arch for c in compilers]
            try:
                android_sdks = self.find_android_sdk(k[2], Options.options.android_sdk_path, archs)
            except Errors.WafError:
                raise
            else:
                for ndk_config, sdk_root, archs, sdk_version in android_sdks:
                    valid_compilers = []
                    seen = set([])
                    for c in compilers:
                        if c.arch in archs and c.arch not in seen:
                            seen.add(c.arch)
                            valid_compilers.append(c)
                    if len(valid_compilers) >= 1:
                        result.append(
                            (
                                valid_compilers[0], valid_compilers,
                                AndroidPlatform(self.conf, ndk_config, sdk_root, sdk_version)
                            )
                        )

        # find all GCC targets
        seen = set([])
        all_gcc_compilers = sorted(compiler_sets.get('gcc', {}).items())
        for k, compilers in all_gcc_compilers:
            if (k[0], k[1]) in seen:
                continue
            for c in compilers:
                prebuilt = os.path.join(k[2], 'prebuilt')
                for target in os.listdir(prebuilt):
                    c.directories.append(os.path.join(prebuilt, target, 'bin'))
            try:
                add_compiler_set(compilers)
            except Errors.WafError as e:
                print(e)
                continue
            else:
                seen.add((k[0], k[1]))

        if all_gcc_compilers:
            for k, compilers in sorted(compiler_sets.get('clang', {}).items()):
                if (k[0], k[1]) in seen:
                    continue
                c = compilers[0]
                clang_compilers = []
                for gcc in all_gcc_compilers[-1][1]:
                    gcc_toolchain = os.path.dirname(os.path.dirname(gcc.compiler_c))
                    extra_args = deepcopy(c.extra_args)
                    extra_args['c'] += ['-target', gcc.target, '-gcc-toolchain', gcc_toolchain]
                    extra_args['cxx'] += ['-target', gcc.target, '-gcc-toolchain', gcc_toolchain]
                    extra_args['link'] += ['-target', gcc.target, '-gcc-toolchain', gcc_toolchain]
                    try:
                        clang_compiler = c.__class__(c.compiler_c, c.compiler_cxx, extra_args)
                    except Exception:
                        pass
                    else:
                        prebuilt = os.path.join(k[2], 'prebuilt')
                        for target in os.listdir(prebuilt):
                            clang_compiler.directories.append(os.path.join(prebuilt, target, 'bin'))
                        clang_compiler.directories += gcc.directories
                        clang_compiler.target = gcc.target
                        clang_compilers.append(clang_compiler)
                if clang_compilers:
                    try:
                        add_compiler_set(clang_compilers)
                    except Errors.WafError as e:
                        print(e)
                        continue
                    else:
                        seen.add((k[0], k[1]))
        else:
            for k, compilers in sorted(compiler_sets.get('clang', {}).items()):
                if (k[0], k[1]) in seen:
                    continue
                c = compilers[0]
                clang_compilers = []
                targets = os.path.normpath(os.path.join(c.compiler_c, '..', '..', 'lib', 'gcc'))
                try:
                    targets = os.listdir(targets)
                except FileNotFoundError:
                    #print(targets)
                    targets = []
                for target in targets:
                    extra_args = deepcopy(c.extra_args)
                    extra_args['c'] += ['-target', target]
                    extra_args['cxx'] += ['-target', target]
                    extra_args['link'] += ['-target', target]
                    try:
                        clang_compiler = c.__class__(c.compiler_c, c.compiler_cxx, extra_args)
                    except Exception:
                        pass
                    else:
                        for path in self.conf.env.EXTRA_PATH:
                            lib_path = os.path.normpath(os.path.join(path, '..', 'lib', 'gcc', target))
                            if os.path.isdir(lib_path):
                                clang_compiler.directories.append(path)
                        prebuilt = os.path.join(k[2], 'prebuilt')
                        for t in os.listdir(prebuilt):
                            clang_compiler.directories.append(os.path.join(prebuilt, t, 'bin'))
                        clang_compiler.target = target
                        clang_compilers.append(clang_compiler)

                if clang_compilers:
                    try:
                        add_compiler_set(clang_compilers)
                    except Errors.WafError as e:
                        print(e)
                        continue
                    else:
                        seen.add((k[0], k[1]))
        return result


def configure(configuration_context):
    if not Options.options.android_sdk_path or not Options.options.android_ndk_path:
        return
    configuration_context.start_msg('Checking for Android tools')
    try:
        configuration_context.platforms.append(AndroidLoader(configuration_context))
    except Errors.WafError as e:
        configuration_context.end_msg(str(e), color='YELLOW')
    else:
        configuration_context.end_msg('done')
