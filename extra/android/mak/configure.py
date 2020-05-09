#! /usr/bin/env python
# set Android specific options

import os
import re
from copy import deepcopy
from waflib import Context, Errors, Logs, Options, Configure


class AndroidPlatform(Configure.ConfigurationContext.Platform):
    NAME = 'android'

    def __init__(self, conf, ndk_root, sdk_root, version):
        Configure.ConfigurationContext.Platform.__init__(self)
        self.NAME = self.__class__.NAME + '_' + self.get_android_version(version)
        self.ndk_path = ndk_root
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
            '27': 'Oreo_8.1'
        }
        return versions.get(sdk_version, 'api'+sdk_version)

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

    def get_android_arch(self, arch):
        archs = {
            'armv7a': 'arm',
            'amd64': 'x86_64',
            'mipsel': 'mips',
            'mips64el': 'mips64'
        }
        return archs.get(arch, arch)

    def get_android_c_flags(self, compiler):
        arch_flags = {
            'gcc': {
                'x86':      [],
                'amd64':    [],
                'armv7a':   ['-march=armv7-a', '-mfloat-abi=softfp', '-mfpu=vfpv3-d16'],
                'arm64':    [],
                'mipsel':   [],
                'mips64el': [],
            },
            'clang': {
                'x86':      [],
                'amd64':    [],
                'armv7a':   ['-fno-integrated-as', '-march=armv7-a', '-mfloat-abi=softfp', '-mfpu=vfpv3-d16'],
                'arm64':    [],
                'mipsel':   ['-fintegrated-as'],
                'mips64el': ['-fintegrated-as'],
            }
        }
        return arch_flags[compiler.NAMES[0].lower()][compiler.arch]

    def get_android_ld_flags(self, compiler):
        arch_flags = {
            'gcc': {
                'x86':      [],
                'amd64':    [],
                'armv7a':   ['-Wl,--fix-cortex-a8', ],
                'arm64':    [],
                'mipsel':   [],
                'mips64el': [],
            },
            'clang': {
                'x86':      [],
                'amd64':    [],
                'armv7a':   ['-Wl,--fix-cortex-a8', ],
                'arm64':    [],
                'mipsel':   [],
                'mips64el': [],
            }
        }
        return arch_flags[compiler.NAMES[0].lower()][compiler.arch]

    def load_in_env(self, conf, compiler):
        env = conf.env
        arch = compiler.arch
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

        env.DEPLOY_ROOTDIR = appname
        env.DEPLOY_BINDIR = os.path.join('lib', target_folder)
        env.DEPLOY_RUNBINDIR = os.path.join('lib', target_folder)
        env.DEPLOY_LIBDIR = os.path.join('lib', target_folder)
        env.DEPLOY_INCLUDEDIR = 'include'
        env.DEPLOY_DATADIR = os.path.join('assets')
        env.DEPLOY_PLUGINDIR = os.path.join('lib', target_folder)
        env.DEPLOY_KERNELDIR = os.path.join('lib', target_folder)

        env.append_value('CFLAGS', self.get_android_c_flags(compiler))
        env.append_value('CXXFLAGS', self.get_android_c_flags(compiler))
        env.append_value('LDFLAGS', self.get_android_ld_flags(compiler))

        env.ANDROID_SDK = self.sdk_version
        env.ANDROID_SDK_PATH = self.sdk_path
        env.ANDROID_NDK_PATH = self.ndk_path
        for n in Options.options.android_ndk_path.split(','):
            n = os.path.normpath(os.path.abspath(n))
            if self.ndk_path.startswith(n):
                ndk_root = n
                break
        sysroot_options = []
        env.ANDROID_ARCH = self.get_android_arch(arch)
        ndk_path = os.path.join(self.ndk_path, 'arch-%s' % env.ANDROID_ARCH)
        conf.env.SYSROOT = ndk_path
        compiler.sysroot = ndk_path
        if os.path.isdir(os.path.join(n, 'sysroot')):
            sysroot_ld_options = ['--sysroot', ndk_path]
            sysroot_options = ['-D__ANDROID_API__=%s' % self.ndk_path.split('-')[-1],
                               '-isystem', os.path.join(ndk_root, 'sysroot', 'usr', 'include'),
                               '-isystem', os.path.join(ndk_root, 'sysroot', 'usr', 'include', compiler.target)]
        else:
            sysroot_ld_options = ['--sysroot', ndk_path]
            sysroot_options = ['--sysroot', ndk_path]
        env.append_unique('JAVACFLAGS', ['-bootclasspath', os.path.join(self.sdk_path, 'android.jar')])
        env.append_unique('AAPTFLAGS', ['-I', os.path.join(self.sdk_path, 'android.jar')])

        if not os.path.isfile(os.path.join(self.ndk_path, '..', '..', 'prebuilt',
                                           'android-%s' % env.ANDROID_ARCH, 'gdbserver', 'gdbserver')):
            raise Errors.WafError('could not find gdbserver for architecture %s' % env.ANDROID_ARCH)

        conf.env.append_value('CFLAGS', sysroot_options)
        conf.env.append_value('CXXFLAGS', sysroot_options)
        conf.env.append_value('LINKFLAGS', sysroot_ld_options)


class AndroidLoader(Configure.ConfigurationContext.Platform):
    NAME = 'android'

    def __init__(self, conf):
        self.conf = conf
        Configure.ConfigurationContext.Platform.__init__(self)

        conf.load('javaw')
        conf.env.append_value('JAVACFLAGS', ['-source', '1.6', '-target', '1.6'])
        conf.find_program('jarsigner', var='JARSIGNER')
        key_debug = conf.path.parent.make_node('debug.keystore')
        conf.env.JARSIGNER_FLAGS = ['-sigalg', 'MD5withRSA', '-digestalg', 'SHA1', '-keystore', key_debug.abspath(),
                                    '-storepass', 'android', '-keypass', 'android']
        conf.env.JARSIGNER_KEY = 'androiddebugkey'

        sdk_build_tool_path = self.get_build_tool_path(Options.options.android_sdk_path)
        sdk_tools_paths = self.get_tools_paths(Options.options.android_sdk_path)
        conf.find_program('adb', path_list=sdk_tools_paths)
        conf.env.DEX = os.path.join(sdk_build_tool_path, 'lib', 'dx.jar')
        if not os.path.isfile(conf.env.DEX):
            raise Errors.WafError('Unable to locate dx.jar')
        conf.find_program('zipalign', var='ZIPALIGN', path_list=sdk_tools_paths+[sdk_build_tool_path])
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

    def get_android_arch(self, arch):
        archs = {
            'armv7a': 'arm',
            'amd64': 'x86_64',
            'mipsel': 'mips',
            'mips64el': 'mips64'
        }
        return archs.get(arch, arch)

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
                a = self.get_android_arch(arch)
                p = os.path.join(platform_ndk, platform, 'arch-%s' % a)
                if os.path.isdir(p):
                    result.append(arch)
            return result

        def ndk_of_sdk(sdk, ndks):
            try:
                sdk_number = int(sdk.split('-')[1])
                ndk_numbers = sorted([(n[0], n[1]) for n in ndks if n[0] <= sdk_number],
                                     key=lambda x: (len(x[1]), x[0]))
                return 'android-%d' % ndk_numbers[-1][0]
            except Exception:
                if sdk in ndks:
                    return sdk

        all_ndk_sdks = []
        platforms_ndk = os.path.join(ndk_path, 'platforms')
        for p in os.listdir(platforms_ndk):
            if os.path.isdir(os.path.join(platforms_ndk, p)):
                all_ndk_sdks.append((int(p.split('-')[1]), valid_archs(platforms_ndk, p, archs)))

        all_sdk_sdks = []
        platforms_sdk = os.path.join(sdk_path, 'platforms')
        all_sdk_sdks = [p for p in os.listdir(platforms_sdk)]
        sdk_pairs = [(i, ndk_of_sdk(i, all_ndk_sdks)) for i in all_sdk_sdks]
        sdk_pairs = [(i, j) for i, j in sdk_pairs if j]
        sdk_pairs = sorted(sdk_pairs, key=lambda x: alphanum_key(x[0]))
        if sdk_pairs:
            prefered_sdk = Options.options.android_sdk
            if prefered_sdk == 'all':
                return [(os.path.join(platforms_ndk, ndk),
                         os.path.join(platforms_sdk, sdk),
                         valid_archs(platforms_ndk, ndk, archs),
                         sdk.split('-')[1]) for sdk, ndk in sdk_pairs]
            elif prefered_sdk:
                if 'android-%s' % prefered_sdk in all_sdk_sdks:
                    sdk = 'android-%s' % prefered_sdk
                    ndk = dict(sdk_pairs)[sdk]
                else:
                    Logs.warn('could not find android SDK version %s in path %s; using %s' % (prefered_sdk,
                                                                                              sdk_path, sdk))
            else:
                sdk, ndk = sdk_pairs[0]
                return [(os.path.join(platforms_ndk, ndk),
                        os.path.join(platforms_sdk, sdk),
                        valid_archs(platforms_ndk, ndk, archs),
                        sdk.split('-')[1])]
        else:
            raise Errors.WafError('no SDK for archs')

    def get_available_compilers(self, configuration_context, compiler_list):
        result = []
        compiler_sets = {}
        for c in compiler_list:
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
                for ndk_root, sdk_root, archs, sdk_version in android_sdks:
                    valid_compilers = [c for c in compilers if c.arch in archs]
                    if len(valid_compilers) > 1:
                        result.append((valid_compilers[0], valid_compilers,
                                      AndroidPlatform(self.conf, ndk_root, sdk_root, sdk_version)))
                    else:
                        result.append((valid_compilers[0], [], AndroidPlatform(self.conf, sdk_version)))

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
                        clang_compiler = c.__class__(c.compiler_c,
                                                     c.compiler_cxx,
                                                     extra_args)
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
                    print(targets)
                    targets = []
                for target in targets:
                    extra_args = deepcopy(c.extra_args)
                    extra_args['c'] += ['-target', target]
                    extra_args['cxx'] += ['-target', target]
                    extra_args['link'] += ['-target', target]
                    try:
                        clang_compiler = c.__class__(c.compiler_c,
                                                     c.compiler_cxx,
                                                     extra_args)
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
