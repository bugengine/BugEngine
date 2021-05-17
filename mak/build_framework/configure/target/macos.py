# set MacOS X specific options

from waflib import Configure, Options, Context, Errors, Utils
from waflib.Configure import conf
import os
import re
import sys
import plist_smalllib
from macholib import MachO


class Darwin(Configure.ConfigurationContext.Platform):
    NAME = 'Darwin'
    SDK_NAME = 'Darwin'
    OS_NAME = 'darwin'
    SUPPORTED_TARGETS = (re.compile('.*-darwin.*'), )

    def __init__(self, conf, sdk=None):
        Configure.ConfigurationContext.Platform.__init__(self)
        self.sdk = sdk
        self.conf = conf
        if sdk:
            self.NAME = self.__class__.NAME + sdk[0]
            self.directories += sdk[2]

    def platform_name(self, compiler):
        return self.__class__.NAME.lower() + self.sdk[0]

    def get_available_compilers(self, configuration_context, compiler_list):
        result = []
        compiler_sets = {}
        for c in compiler_list:
            for r in self.SUPPORTED_TARGETS:
                if r.match(c.target):
                    k = (c.NAMES[0], c.version)
                    try:
                        compiler_sets[k].append(c)
                    except KeyError:
                        compiler_sets[k] = [c]
                    break
        if compiler_sets:
            self.conf.start_msg('Looking for %s SDKs' % self.SDK_NAME)
            for k in sorted(compiler_sets.keys()):
                compilers = compiler_sets[k]
                try:
                    compilers, sdk = self.get_best_compilers_sdk(compilers)
                except Errors.WafError as e:
                    continue
                else:
                    if len(compilers) > 1:
                        result.append((compilers[0], compilers, self.__class__(self.conf, sdk)))
                    else:
                        result.append((compilers[0], [], self.__class__(self.conf, sdk)))
            if result:
                self.conf.end_msg('done')
            else:
                self.conf.end_msg('none', color='YELLOW')
        return result

    def get_root_dirs(self, appname):
        return (os.path.join(appname + '.app', 'Contents'), os.path.join(appname + '.app', 'Contents', 'MacOS'))

    def load_in_env(self, conf, compiler):
        platform = self.SDK_NAME.lower()
        compiler.find_target_program(conf, self, 'lipo', mandatory=False) or conf.find_program('lipo')
        compiler.find_target_program(conf, self, 'codesign', mandatory=False)
        if not conf.env.CODESIGN:
            conf.find_program('codesign', mandatory=False)
        compiler.find_target_program(conf, self, 'dsymutil', mandatory=False)
        if not conf.env.DSYMUTIL:
            conf.find_program('dsymutil', mandatory=False)
        if not conf.env.DSYMUTIL:
            compiler.find_target_program(conf, self, 'llvm-dsymutil', var='DSYMUTIL', mandatory=False)
        if not conf.env.DSYMUTIL:
            conf.find_program('llvm-dsymutil', var='DSYMUTIL', mandatory=False)
        environ = getattr(conf, 'environ', os.environ)
        conf.env.PATH = os.path.pathsep.join(self.directories + compiler.directories + [environ['PATH']])
        conf.env.ABI = 'mach_o'
        conf.env.COMPILER_ABI = 'gcc'
        conf.env.VALID_PLATFORMS = self.PLATFORMS + ['darwin']
        conf.env.XCODE_SDKROOT = platform
        conf.env.XCODE_ABI = 'mach_o'
        conf.env.XCODE_SUPPORTEDPLATFORMS = platform
        conf.env.CFLAGS_cshlib = ['-fPIC']
        conf.env.CXXFLAGS_cxxshlib = ['-fPIC']
        conf.env.LINKFLAGS_cshlib = ['-dynamiclib']
        conf.env.LINKFLAGS_cxxshlib = ['-dynamiclib']
        conf.env.cshlib_PATTERN = 'lib%s.dylib'
        conf.env.cxxshlib_PATTERN = 'lib%s.dylib'
        conf.env.FRAMEWORKPATH_ST = '-F%s'
        conf.env.FRAMEWORK_ST = ['-framework']
        conf.env.ARCH_ST = ['-arch']

        conf.env.LINKFLAGS_cxxstlib = []

        conf.env.SHLIB_MARKER = []
        conf.env.STLIB_MARKER = []
        conf.env.SONAME_ST = []
        conf.env.pymodule_PATTERN = '%s.so'

        appname = getattr(Context.g_module, Context.APPNAME, conf.srcnode.name)
        root_dir, bin_dir = self.get_root_dirs(appname)
        conf.env.DEPLOY_ROOTDIR = root_dir
        conf.env.DEPLOY_BINDIR = bin_dir
        conf.env.DEPLOY_RUNBINDIR = bin_dir
        conf.env.DEPLOY_LIBDIR = 'lib'
        conf.env.DEPLOY_INCLUDEDIR = 'include'
        conf.env.DEPLOY_DATADIR = os.path.join(root_dir, 'data')
        conf.env.DEPLOY_PLUGINDIR = os.path.join(root_dir, 'plugins')
        conf.env.DEPLOY_KERNELDIR = os.path.join(root_dir, 'kernels')
        conf.env.append_unique('DEFINES', ['_BSD_SOURCE'])

        conf.env.MACOSX_SDK = os.path.splitext(os.path.basename(self.sdk[1]))[0]
        conf.env.XCODE_SDK_PATH = self.sdk[1]
        conf.env.SYSROOT = self.sdk[1]
        conf.env.MACOSX_SDK_MIN = self.sdk[3]
        conf.env.append_unique('CPPFLAGS', ['-isysroot', self.sdk[1]])
        conf.env.append_unique('CFLAGS', ['-isysroot', self.sdk[1]] + self.sdk[4][0])
        conf.env.append_unique('CXXFLAGS', ['-isysroot', self.sdk[1]] + self.sdk[4][1])
        conf.env.append_unique(
            'LINKFLAGS', ['-isysroot', self.sdk[1], '-L%s/usr/lib' % self.sdk[1]] + self.sdk[4][2] +
            ['-B%s' % bin_path for bin_path in self.directories]
        )
        if compiler.arch == 'x86':
            conf.env.append_unique('CFLAGS', ['-msse2'])
            conf.env.append_unique('CXXFLAGS', ['-msse2'])
        conf.env.env = dict(os.environ)

    platform_sdk_re = re.compile('.*/Platforms/\w*\.platform/Developer/SDKs/[\.\w]*\.sdk')
    old_sdk_re = re.compile('.*/SDKs/[\.\w]*\.sdk')

    def match(self, compiler, sdk_path, all_sdks):
        def get_paths(sdk_path):
            if self.platform_sdk_re.match(sdk_path):
                platform_path = os.path.normpath(os.path.dirname(os.path.dirname(os.path.dirname(sdk_path))))
                developer_path, d = os.path.split(platform_path)
                while d and not d.startswith('Developer'):
                    developer_path, d = os.path.split(developer_path)
            elif self.old_sdk_re.match(sdk_path):
                platform_path = os.path.normpath(os.path.dirname(os.path.dirname(sdk_path)))
                developer_path, d = os.path.split(platform_path)
            else:
                platform_path = os.path.normpath(os.path.dirname(os.path.dirname(sdk_path)))
                developer_path, d = os.path.split(platform_path)
            developer_path = os.path.join(developer_path, d)
            if platform_path[-1] != '/':
                platform_path += '/'
            if developer_path[-1] != '/':
                developer_path += '/'
            return platform_path, developer_path

        compiler_path = os.path.normpath(compiler.compiler_c)
        platform_path, developer_path = get_paths(sdk_path)
        if compiler_path.startswith(platform_path):
            return True
        for sdk_version, sdk_archs, sdk_path in all_sdks:
            ppath, dpath = get_paths(sdk_path)
            if compiler_path.startswith(ppath):
                return False
        if compiler_path.startswith(developer_path):
            return True
        for sdk_version, sdk_archs, sdk_path in all_sdks:
            ppath, dpath = get_paths(sdk_path)
            if compiler_path.startswith(dpath):
                return False
        return True

    def get_best_compilers_sdk(self, compilers):
        src_node = self.conf.bldnode.make_node('test.mm')
        src_node.write(
            '#include <stdio.h>\n'
            '#include <iostream>\n'
            '#include <Foundation/NSObject.h>\n'
            '#include <CoreFoundation/CoreFoundation.h>\n'
            '@interface BugObject : NSObject {\n'
            '}\n'
            '- (id) init;\n'
            '@end\n'
            '@implementation BugObject\n'
            '- (id) init {\n'
            ' self = [super init];\n'
            ' return self;\n'
            '}\n'
            '@end\n'
            'int main(int argc, char* argv[])\n'
            '{\n'
            '    [[BugObject alloc] init];\n'
            '    switch(argc) {\n'
            '    case 6:\n'
            '        return printf("%s\\n", argv[5]);\n'
            '    case 5:\n'
            '        return printf("%s\\n", argv[4]);\n'
            '    case 4:\n'
            '        return printf("%s\\n", argv[3]);\n'
            '    case 3:\n'
            '        return printf("%s\\n", argv[2]);\n'
            '    case 2:\n'
            '        return printf("%s\\n", argv[1]);\n'
            '    default:\n'
            '        return printf("%s\\n", argv[0]);\n'
            '    }\n'
            '}\n'
        )
        obj_node = src_node.change_ext('')
        exe_node = src_node.change_ext('')
        all_archs = []
        for compiler in sorted(compilers, key=lambda x: x.name()):
            if compiler.arch not in all_archs:
                all_archs.append(compiler.arch)

        try:
            sdk_number_min = getattr(Options.options, '%s_sdk_min' % self.OS_NAME)
        except AttributeError:
            sdk_number_min = ''
        sdk_number_min = sdk_number_min and sdk_number_min.split('.') or []
        try:
            sdk_number_max = getattr(Options.options, '%s_sdk_max' % self.OS_NAME)
        except AttributeError:
            sdk_number_max = ''
        sdk_number_max = sdk_number_max and sdk_number_max.split('.') or []
        sdks = []
        relpath = os.path.join('Platforms', '%s.platform' % self.SDK_NAME, 'Developer', 'SDKs')

        try:
            all_sdks = getattr(self.conf, 'darwin_sdks', {})[self.SDK_NAME]
        except KeyError:
            raise Errors.WafError('No SDK detected for platform %s' % self.SDK_NAME)
        else:
            best_sdk = (None, None, [], [], [])
            for sdk_version, sdk_archs, sdk_path in all_sdks:
                if len(best_sdk[2]) >= len(sdk_archs):
                    break
                os_version_min = getattr(self, 'OS_VERSION_MIN', sdk_version)
                os_version_min_libcpp = getattr(self, 'OS_VERSION_MIN_LIBCPP', sdk_version)
                sdk_option = '-m%s-version-min=%s' % (self.OS_NAME, '.'.join(os_version_min))
                if sdk_number_max and sdk_version > sdk_number_max:
                    continue
                if sdk_number_min and sdk_version < sdk_number_min:
                    continue
                sdk_compilers = []
                sdk_bin_paths = [
                    i for i in [
                        os.path.normpath(os.path.join(sdk_path, 'usr', 'bin')),
                        os.path.normpath(os.path.join(sdk_path, '..', '..', 'usr', 'bin')),
                        os.path.normpath(os.path.join(sdk_path, '..', '..', '..', '..', '..', 'usr', 'bin')),
                        os.path.normpath(
                            os.path.join(
                                sdk_path, '..', '..', '..', '..', '..', 'Toolchains', 'XcodeDefault.xctoolchain', 'usr',
                                'bin'
                            )
                        ),
                    ] if os.path.isdir(i) and i != '/usr/bin'
                ]
                strip = self.conf.detect_executable('strip', path_list=sdk_bin_paths)
                if strip is None:
                    otool = self.conf.detect_executable('otool', path_list=sdk_bin_paths)
                    if otool is None:
                        otool = self.conf.detect_executable('otool')
                    if otool is not None:
                        strip = self.conf.detect_executable('strip', path_list=os.path.dirname(otool))

                cflags = []
                cxxflags = []
                ldflags = []
                for libcpp in ('libstdc++-static.a', 'libstdc++.6.dylib', 'libstdc++.tbd'):
                    if os.path.isfile(os.path.join(sdk_path, 'usr', 'lib', libcpp)):
                        break
                else:
                    sdk_option = '-m%s-version-min=%s' % (self.OS_NAME, '.'.join(os_version_min_libcpp))
                    cxxflags.append('-stdlib=libc++')
                    ldflags.append('-stdlib=libc++')
                cflags.append(sdk_option)
                cxxflags.append(sdk_option)
                ldflags.append(sdk_option)

                for a in sdk_archs:
                    for c in compilers:
                        if self.match(c, sdk_path, all_sdks) and c.arch == c.ARCHS[a]:
                            try:
                                obj_node.delete()
                            except Exception:
                                pass
                            try:
                                exe_node.delete()
                            except Exception:
                                pass
                            env = dict(os.environ)
                            env['PATH'] = os.path.pathsep.join(sdk_bin_paths + c.directories + [env['PATH']])
                            r, out, err = c.run_cxx(
                                [
                                    sdk_option, '-g', '-O2', '-c', '-o',
                                    obj_node.abspath(), '-isysroot', sdk_path,
                                    src_node.abspath(),
                                    '-B%s' % os.path.dirname(strip)
                                ] + cxxflags,
                                env=env
                            )
                            if r == 0:
                                r, out, err = c.run([strip, '-S', obj_node.abspath()], env=env)
                                if r == 0:
                                    r, out, err = c.run_cxx(
                                        [
                                            sdk_option, '-framework', 'Foundation', '-framework', 'CoreFoundation',
                                            '-o',
                                            exe_node.abspath(),
                                            '-L%s' % os.path.join(sdk_path, 'usr', 'lib'), '-isysroot', sdk_path,
                                            obj_node.abspath(), '-lobjc',
                                            '-B%s' % os.path.dirname(strip), '-Wl,-rpath,rpath_test'
                                        ] + cxxflags,
                                        env=env
                                    )
                                    if r == 0:
                                        r, out, err = c.run([strip, '-S', exe_node.abspath()], env=env)
                                        if r == 0:
                                            sdk_compilers.append(c)
                                            break
                if len(sdk_compilers) > len(best_sdk[2]):
                    best_sdk = (
                        '.'.join(sdk_version), sdk_path, sdk_compilers, [os.path.dirname(strip)] + sdk_bin_paths,
                        (cflags, cxxflags, ldflags)
                    )
            if best_sdk[2]:
                return best_sdk[2], (best_sdk[0], best_sdk[1], best_sdk[3], '.'.join(os_version_min), best_sdk[4])
            else:
                raise Errors.WafError('No SDK for compiler %s' % compilers[0].compiler_c)


Configure.ConfigurationContext.Darwin = Darwin


class MacOS(Darwin):
    NAME = 'MacOS'
    CERTIFICATE_NAME = 'Mac Developer'
    PLATFORMS = ['macos', 'macosx', 'pc', 'darwin']
    SDK_NAME = 'macosx'
    OS_NAME = 'macosx'
    OS_VERSION_MIN = ('10', '5')
    OS_VERSION_MIN_LIBCPP = ('10', '7')

    def __init__(self, conf, sdk=None):
        Darwin.__init__(self, conf, sdk)

    def load_in_env(self, conf, compiler):
        Darwin.load_in_env(self, conf, compiler)
        sdk_path = self.sdk[1]
        if os.path.isfile(os.path.join(sdk_path, 'usr', 'lib', 'libgcc_s.10.5.dylib')):
            conf.env.append_unique('LINKFLAGS', ['-lgcc_s.10.5'])


def run_command(cmd, input=None, env=None):
    try:
        p = Utils.subprocess.Popen(
            cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE, env=env
        )
        if input is not None:
            p.stdin.write(input.encode())
        out, err = p.communicate()
    except Exception as e:
        return (-1, '', str(e))
    else:
        if not isinstance(out, str):
            out = out.decode(sys.stdout.encoding, errors='ignore')
        if not isinstance(err, str):
            err = err.decode(sys.stderr.encoding, errors='ignore')
        return (p.returncode, out, err)


def parse_sdk_settings(sdk_settings_path):
    with open(sdk_settings_path, 'rb') as sdk_settings_file:
        settings = plist_smalllib.load(sdk_settings_file)
    sdk_name = settings['DefaultProperties']['PLATFORM_NAME']
    sdk_version = settings['Version']
    sdk_version = [str(x) for x in sdk_version.split('.')]
    return str(sdk_name), sdk_version


_CPU_ARCH_ABI64 = 0x01000000

_CPU_TYPE_NAMES = {
    -1: "any",
    1: "vax",
    6: "mc680x0",
    7: "x86",
    _CPU_ARCH_ABI64 | 7: "x86_64",
    8: "mips",
    10: "mc98000",
    11: "hppa",
    12: "arm",
    _CPU_ARCH_ABI64 | 12: "arm64",
    13: "mc88000",
    14: "sparc",
    15: "i860",
    16: "alpha",
    18: "ppc",
    _CPU_ARCH_ABI64 | 18: "ppc64",
}

_CPU_SUBTYPE_NAMES = {
    'arm':
        {
            5: 'v4t',
            6: 'v6',
            7: 'v5tej',
            8: 'xscale',
            9: 'v7',
            10: 'v7f',
            11: 'v7s',
            12: 'v7k',
            13: 'v8',
            14: 'v6m',
            15: 'v7m',
            16: 'v7em',
        },
    'arm64': {
        1: 'v8'
    },
}


def configure(conf):
    archs = {
        'ppc': 'ppc',
        'ppc_7400': 'ppc',
        'ppc64': 'ppc64',
        'x86_64': 'amd64',
        'i386': 'x86',
        'arm_v6': 'armv6',
        'armv7': 'armv7a',
        'arm_v7': 'armv7a',
        'armv7s': 'armv7s',
        'arm_v7s': 'armv7s',
        'armv7k': 'armv7k',
        'arm_v7k': 'armv7k',
        'arm64': 'arm64',
        'arm64e': 'arm64e',
        'arm64_32': 'arm64_32',
    }

    conf.darwin_sdks = {}
    sdks = []
    for p in getattr(conf, 'os_sdk_paths', []):
        for sdk in os.listdir(p):
            sdk_path = os.path.join(p, sdk)
            if os.path.isfile(os.path.join(p, sdk, 'SDKSettings.plist')):
                sdks.append(sdk_path)
    for sysroot, _ in conf.env.SYSROOTS:
        if os.path.isfile(os.path.join(sysroot, 'SDKSettings.plist')):
            sdks.append(sysroot)

    for sdk_path in sdks:
        sdk_os, sdk_version = parse_sdk_settings(os.path.join(sdk_path, 'SDKSettings.plist'))
        sdk_archs = []
        dylib = os.path.join(sdk_path, 'usr', 'lib', 'libc.dylib')
        if os.path.isfile(dylib):
            mach_o = MachO.MachO(dylib)
            for header in mach_o.headers:
                arch_name = _CPU_TYPE_NAMES.get(header.header.cputype, 'unknown')
                arch_name += _CPU_SUBTYPE_NAMES.get(arch_name, {}).get(header.header.cpusubtype, '')
                sdk_archs.append(arch_name)
        else:
            tbd = os.path.join(sdk_path, 'usr', 'lib', 'libc.tbd')
            if os.path.isfile(tbd):
                with open(tbd, 'r') as tbd_file:
                    for line in tbd_file.readlines():
                        line = line.strip()
                        if line.startswith('archs:'):
                            line = line.split()
                            for a in line[2:-1]:
                                try:
                                    sdk_archs.append(archs[a.split(',')[0]])
                                except KeyError:
                                    print('Unknown %s arch: %s in %s' % (sdk_os, a, dylib))
                            break
                        elif line.startswith('targets:'):
                            line = line.split()
                            for t in line[2:-1]:
                                try:
                                    sdk_archs.append(archs[t.split('-')[0]])
                                except KeyError:
                                    print('Unknown %s target: %s in %s' % (sdk_os, t, dylib))
                            break
        try:
            conf.darwin_sdks[sdk_os].append((sdk_version, sdk_archs, sdk_path))
            conf.darwin_sdks[sdk_os].sort()
        except KeyError:
            conf.darwin_sdks[sdk_os] = [(sdk_version, sdk_archs, sdk_path)]

    for sdk_os in conf.darwin_sdks.keys():
        conf.darwin_sdks[sdk_os] = sorted(conf.darwin_sdks[sdk_os], key=lambda x: (-len(x[1]), x[0]))

    conf.platforms.append(MacOS(conf))
