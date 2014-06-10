# set MacOS X specific options

import os
from waflib import Context, Errors, Utils, Task, Options
from waflib.Logs import info,warn,pprint
from waflib.TaskGen import feature, before_method, after_method, extension
from waflib.Configure import conf


@conf
def set_macosx_options(self):
    self.env.ABI = 'mach_o'
    self.env.VALID_PLATFORMS = ['macosx', 'darwin', 'pc']
    self.env.XCODE_SDKROOT = 'macosx'
    self.env.XCODE_ABI = 'mach_o'
    self.env.XCODE_SUPPORTEDPLATFORMS = 'macosx'
    self.env.pymodule_PATTERN = '%s.so'

    appname = getattr(Context.g_module, Context.APPNAME, self.srcnode.name)
    self.env.DEPLOY_ROOTDIR = os.path.join(appname + '.app', 'Contents')
    self.env.DEPLOY_BINDIR = os.path.join(appname + '.app', 'Contents', 'MacOS')
    self.env.DEPLOY_RUNBINDIR = os.path.join(appname + '.app', 'Contents', 'MacOS')
    self.env.DEPLOY_LIBDIR = 'lib'
    self.env.DEPLOY_INCLUDEDIR = 'include'
    self.env.DEPLOY_DATADIR = os.path.join(appname + '.app', 'Contents', 'share', 'bugengine')
    self.env.DEPLOY_PLUGINDIR = os.path.join(appname + '.app', 'Contents', 'share', 'bugengine', 'plugin')
    self.env.DEPLOY_KERNELDIR = os.path.join(appname + '.app', 'Contents', 'share', 'bugengine', 'kernel')


@conf
def check_multilib_gcc(self, compiler):
    flags = compiler[8]
    arch = compiler[7]
    if flags[0] == '-m64':
        filename = os.path.join(self.bldnode.abspath(), 'main.c')
        output = os.path.join(self.bldnode.abspath(), 'main.o')
        with open(filename, 'w') as f:
            f.write('int main(int, const char**) { return 0; }')
            cmd = [os.path.join(compiler[0], compiler[3]), '-Wp,-imultilib,%s'%arch, '-c', filename, '-o', output]
            try:
                p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE)
                out = p.communicate()[0]
            except Exception as e:
                print(e)
            else:
                if p.returncode == 0:
                    flags.append('-Wp,-imultilib,%s'%arch)


@conf
def check_multilib_other(self, compiler):
    pass


@conf
def get_macosx_sdk_version(self, name, compiler_list):
    sdk_number_min = Options.options.macosx_sdk_min
    sdk_number_max = Options.options.macosx_sdk_max
    sdks = []
    paths = Options.options.xcode_sdks.split(',')
    for path in paths:
        for sdk_relative_path in [os.path.join('Platforms', 'MacOSX.platform', 'Developer', 'SDKs'), 'SDKs']:
            sdks_path = os.path.join(path, sdk_relative_path)
            try:
                sdk_list = os.listdir(sdks_path)
            except OSError:
                continue
            else:
                for sdk in sdk_list:
                    sdk_name,_ = os.path.splitext(sdk)
                    if sdk.startswith('MacOSX'):
                        sdk_version = sdk_name[6:10]
                        sdks.append((sdk_version, os.path.join(sdks_path, sdk)))

    sdks.sort()
    self.start_msg('Looking for SDK for %s-%s'%name)
    for sdk, sdk_path in sdks:
        bin_paths = []
        if sdk_number_max and not sdk <= sdk_number_max:
            continue
        if sdk_number_min and not sdk >= sdk_number_min:
            continue
        try:
            for compiler in compiler_list:
                self.check_sdk(os.path.join(compiler[1], compiler[3]), compiler[8], sdk_path,
                               '-mmacosx-version-min=%s'%sdk, ['Cocoa'])
                bin_paths.append(compiler[1])
                bin_paths.append(os.path.normpath(os.path.join(sdk_path, '..', '..', 'usr', 'bin')))
                bin_paths.append(os.path.normpath(os.path.join(sdk_path, '..', '..', '..', 'usr', 'bin')))
                bin_paths.append(os.path.normpath(os.path.join(sdk_path, '..', '..', '..', '..', '..', 'usr', 'bin')))
            break
        except Errors.WafError:
            continue
    else:
        self.end_msg('none', color='RED')
        raise Errors.WafError('Could not find any suitable SDK')
    self.end_msg(os.path.splitext(os.path.basename(sdk_path))[0])
    return (sdk, sdk_path, list(set(bin_paths)))


@conf
def set_macosx_sdk_options(self, sdk_version, sdk_path):
    self.env.MACOSX_SDK = os.path.splitext(os.path.basename(sdk_path))[0]
    self.env.XCODE_SDK_PATH = sdk_path
    self.env.SYSROOT = [sdk_path]
    self.env.append_unique('CFLAGS', ['-mmacosx-version-min=%s'%sdk_version, '-isysroot', sdk_path])
    self.env.append_unique('CXXFLAGS', ['-mmacosx-version-min=%s'%sdk_version, '-isysroot', sdk_path])
    self.env.append_unique('LINKFLAGS', ['-mmacosx-version-min=%s'%sdk_version, '-isysroot', sdk_path, '-L%s/usr/lib'%sdk_path])


@conf
def set_macosx_icc_options(self, flags, arch):
    v = self.env
    v.CFLAGS = flags + ['-fPIC', '-fvisibility=hidden']
    v.CXXFLAGS = flags + ['-fPIC', '-fvisibility=hidden']
    v.LINKFLAGS = flags + ['-lobjc']
    v.CXXFLAGS_cxxshlib = []
    v.CFLAGS_exportall = ['-fvisibility=default']
    v.CXXFLAGS_exportall = ['-fvisibility=default']

    v.CFLAGS_warnnone = ['-w']
    v.CXXFLAGS_warnnone = ['-w']
    v.CFLAGS_warnall = ['-std=c99', '-Wall', '-Wextra', '-pedantic', '-Winline', '-Werror']
    v.CXXFLAGS_warnall = ['-Wall', '-Wextra', '-Werror', '-Wno-sign-compare', '-Woverloaded-virtual', '-Wno-invalid-offsetof']

    v.CFLAGS_debug = ['-pipe', '-g', '-D_DEBUG']
    v.CXXFLAGS_debug = ['-pipe', '-g', '-D_DEBUG']
    v.ASFLAGS_debug = ['-pipe', '-g', '-D_DEBUG']
    v.LINKFLAGS_debug = ['-pipe', '-g']

    v.CFLAGS_profile = ['-pipe', '-g', '-DNDEBUG', '-O3']
    v.ASFLAGS_profile = ['-pipe', '-g', '-DNDEBUG', '-O3']
    v.LINKFLAGS_profile = ['-pipe', '-g']

    v.CFLAGS_final = ['-pipe', '-g', '-DNDEBUG', '-O3']
    v.CXXFLAGS_final = ['-pipe', '-Wno-unused-parameter', '-g', '-DNDEBUG', '-O3', '-fno-rtti', '-fno-exceptions']
    v.ASFLAGS_final = ['-pipe', '-g', '-DNDEBUG', '-O3']
    v.LINKFLAGS_final = ['-pipe', '-g']

    self.env.append_unique('LINKFLAGS_cshlib', ['-undefined', 'dynamic_lookup', '-dynamiclib'])
    self.env.append_unique('LINKFLAGS_cxxshlib', ['-undefined', 'dynamic_lookup', '-dynamiclib'])


@conf
def set_macosx_gcc_options(self, flags, arch):
    v = self.env
    v.CFLAGS = flags + ['-fPIC', '-fvisibility=hidden']
    v.CXXFLAGS = flags + ['-fPIC', '-fvisibility=hidden']
    v.LINKFLAGS = flags + ['-lobjc']
    v.CXXFLAGS_cxxshlib = []
    v.CFLAGS_exportall = ['-fvisibility=default']
    v.CXXFLAGS_exportall = ['-fvisibility=default']

    v.CFLAGS_warnnone = ['-w']
    v.CXXFLAGS_warnnone = ['-w']
    v.CFLAGS_warnall = ['-std=c99', '-Wall', '-Wextra', '-pedantic', '-Winline', '-Werror']
    v.CXXFLAGS_warnall = ['-Wall', '-Wextra', '-Werror', '-Wno-sign-compare', '-Woverloaded-virtual', '-Wno-invalid-offsetof']

    v.CFLAGS_debug = ['-pipe', '-g', '-D_DEBUG']
    v.CXXFLAGS_debug = ['-pipe', '-g', '-D_DEBUG']
    v.ASFLAGS_debug = ['-pipe', '-g', '-D_DEBUG']
    v.LINKFLAGS_debug = ['-pipe', '-g']

    v.CFLAGS_profile = ['-pipe', '-g', '-DNDEBUG', '-O3']
    v.CXXFLAGS_profile = ['-pipe', '-Wno-unused-parameter', '-g', '-DNDEBUG', '-O3', '-fno-rtti', '-fno-exceptions']
    v.ASFLAGS_profile = ['-pipe', '-g', '-DNDEBUG', '-O3']
    v.LINKFLAGS_profile = ['-pipe', '-g']

    v.CFLAGS_final = ['-pipe', '-g', '-DNDEBUG', '-O3']
    v.CXXFLAGS_final = ['-pipe', '-Wno-unused-parameter', '-g', '-DNDEBUG', '-O3', '-fno-rtti', '-fno-exceptions']
    v.ASFLAGS_final = ['-pipe', '-g', '-DNDEBUG', '-O3']
    v.LINKFLAGS_final = ['-pipe', '-g']

    self.env.append_unique('LINKFLAGS_cshlib', ['-undefined', 'dynamic_lookup', '-dynamiclib'])
    self.env.append_unique('LINKFLAGS_cxxshlib', ['-undefined', 'dynamic_lookup', '-dynamiclib'])


@conf
def set_macosx_clang_options(self, flags, arch):
    v = self.env
    v.CFLAGS = flags + ['-fPIC', '-fvisibility=hidden']
    v.CXXFLAGS = flags + ['-fPIC', '-fvisibility=hidden']
    v.LINKFLAGS = flags + ['-lobjc']
    v.CXXFLAGS_cxxshlib = []
    v.CFLAGS_exportall = ['-fvisibility=default']
    v.CXXFLAGS_exportall = ['-fvisibility=default']

    v.CFLAGS_warnnone = ['-w']
    v.CXXFLAGS_warnnone = ['-w']
    v.CFLAGS_warnall = ['-std=c99', '-Wall', '-Wextra', '-pedantic', '-Winline', '-Werror']
    v.CXXFLAGS_warnall = ['-Wall', '-Wextra', '-Werror', '-Wno-sign-compare', '-Woverloaded-virtual', '-Wno-invalid-offsetof']

    v.CFLAGS_debug = ['-pipe', '-g', '-D_DEBUG']
    v.CXXFLAGS_debug = ['-pipe', '-g', '-D_DEBUG']
    v.ASFLAGS_debug = ['-pipe', '-g', '-D_DEBUG']
    v.LINKFLAGS_debug = ['-pipe', '-g']

    v.CFLAGS_profile = ['-pipe', '-g', '-DNDEBUG', '-O3']
    v.CXXFLAGS_profile = ['-pipe', '-Wno-unused-parameter', '-g', '-DNDEBUG', '-O3', '-fno-rtti', '-fno-exceptions']
    v.ASFLAGS_profile = ['-pipe', '-g', '-DNDEBUG', '-O3']
    v.LINKFLAGS_profile = ['-pipe', '-g']

    v.CFLAGS_final = ['-pipe', '-g', '-DNDEBUG', '-O3']
    v.CXXFLAGS_final = ['-pipe', '-Wno-unused-parameter', '-g', '-DNDEBUG', '-O3', '-fno-rtti', '-fno-exceptions']
    v.ASFLAGS_final = ['-pipe', '-g', '-DNDEBUG', '-O3']
    v.LINKFLAGS_final = ['-pipe', '-g']

    self.env.append_unique('LINKFLAGS_cshlib', ['-undefined', 'dynamic_lookup', '-dynamiclib'])
    self.env.append_unique('LINKFLAGS_cxxshlib', ['-undefined', 'dynamic_lookup', '-dynamiclib'])


def options(opt):
    gr = opt.get_option_group('SDK paths and options')
    gr.add_option( '--macosx-sdk-min',
                    action='store',
                    default='',
                    dest='macosx_sdk_min',
                    help='Minimum version of the MacOS X SDK to target')
    gr.add_option( '--macosx-sdk-max',
                    action='store',
                    default='',
                    dest='macosx_sdk_max',
                    help='Maximum version of the MacOS X SDK to target')


def configure(conf):
    supported_architectures ={
        'x86' : 'i686',
        'i386' : 'i686',
        'i686': 'i686',
        'x86_64' : 'x86_64',
        'x64' : 'x86_64',
        'amd64' : 'x86_64',
        'ppc' : 'ppc',
        'powerpc' : 'ppc',
        'ppc64' : 'ppc64',
        'powerpc64' : 'ppc64'
    }

    def load_gcc(conf, compiler):
        conf.load_gcc(compiler[1], compiler[2], compiler[3], compiler[4], compiler[5], compiler[6], compiler[8])
    def load_icc(conf, compiler):
        conf.load_icc(compiler[1], compiler[2], compiler[3], compiler[4], compiler[5], compiler[6], compiler[8])
    def load_clang(conf, compiler):
        conf.load_clang(compiler[1], compiler[5], compiler[8])

    compiler_versions = {}
    for name, bindir, icc, ixx, version, target, arch, options in conf.env.ICC_TARGETS:
        position = target.find('darwin')
        if position != -1 and arch in supported_architectures:
            key = (name, version, check_multilib_other, load_icc, set_macosx_icc_options)
            compiler = (name, bindir, icc, ixx, version, target, arch, supported_architectures[arch], options)
            try:
                compiler_versions[key].append(compiler)
            except KeyError:
                compiler_versions[key] = [compiler]
    for name, bindir, gcc, gxx, version, target, arch, options in conf.env.GCC_TARGETS:
        position = target.find('darwin')
        if position != -1 and arch in supported_architectures:
            version = target[position+6:].split('-')[0] + '_' + version
            key = (name, version, check_multilib_gcc, load_gcc, set_macosx_gcc_options)
            compiler = (name, bindir, gcc, gxx, version, target, arch, supported_architectures[arch], options)
            try:
                compiler_versions[key].append(compiler)
            except KeyError:
                compiler_versions[key] = [compiler]
    for version, directory, target, arch in conf.env.CLANG_TARGETS:
        real_arch, flags = arch
        position = target.find('darwin')
        if position != -1 and real_arch in supported_architectures:
            key = ('clang', version, check_multilib_other, load_clang, set_macosx_clang_options)
            compiler = ('clang', directory, 'clang', 'clang++', version, target, real_arch, supported_architectures[real_arch], flags)
            try:
                compiler_versions[key].append(compiler)
            except KeyError:
                compiler_versions[key] = [compiler]

    seen = set([])
    def get_version(x):
        x = x.split('_')
        key = 0.
        div = 1.
        for y in x[0].split('.'):
            key += float(y) / div
            div *= 100
        if len(x) > 1:
            return key, x[1]
        else:
            return key, ''
    for key, compiler_list in sorted(compiler_versions.items(), key=lambda x: (x[0][0], get_version(x[0][1]))):
        macosx_toolchains = []
        macosx_archs = []
        name, version, check_multilib, load_compiler, set_macosx_compiler_options = key
        if (name, version) in seen:
            continue
        seen.add((name, version))
        for compiler in compiler_list:
            check_multilib(conf, compiler)
        try:
            sdk_name, sdk_path, bin_paths = conf.get_macosx_sdk_version((key[0], key[1]), compiler_list)
        except Errors.WafError:
            continue
        for compiler in compiler_list:
            name, bindir, gcc, gxx, version, target, arch, real_arch, options = compiler
            os_name = 'macosx'
            toolchain = '%s-%s-%s-%s'%(os_name, arch, name, version)
            if toolchain in seen:
                continue
            seen.add(toolchain)
            env = conf.env.derive()
            conf.setenv(toolchain, env)
            env.DEST_OS = 'darwin'
            try:
                conf.start_msg('Setting up compiler')
                load_compiler(conf, compiler)
                set_macosx_compiler_options(conf, options, real_arch)
            except Exception as e:
                conf.end_msg(e, color='RED')
                raise
            else:
                conf.end_msg('done')
                try:
                    conf.set_macosx_options()
                    conf.set_macosx_sdk_options(sdk_name, sdk_path)
                    toolchain = '%s-%s-%s-%s'%(os_name+conf.env.MACOSX_SDK[6:], real_arch, name, version)
                    conf.setenv(toolchain, conf.env.derive())
                    conf.env.KERNEL_TOOLCHAINS = [toolchain]
                    conf.env.ENV_PREFIX = real_arch
                    conf.add_toolchain(os_name+conf.env.MACOSX_SDK[6:], real_arch, name, version, arch, False)
                except Errors.WafError as e:
                    conf.variant = ''
                    pprint('YELLOW', '%s failed: %s' % (toolchain, e))
                except Exception as e:
                    conf.variant = ''
                    pprint('RED', '%s failed: %s' % (toolchain, e))
                    raise
                else:
                    macosx_toolchains.append(toolchain)
                    macosx_archs.append(real_arch)
                    conf.variant = ''
        if macosx_toolchains:
            toolchain = '%s%s-%s-%s-%s'%(os_name, sdk_name, ','.join(macosx_archs), name, version)
            env = conf.env.derive()
            conf.setenv(toolchain, env)
            conf.set_macosx_options()
            conf.env.SUB_TOOLCHAINS = macosx_toolchains
            conf.env.VALID_ARCHITECTURES = [macosx_archs]
            conf.find_program('dsymutil', path_list=bin_paths)
            conf.find_program('strip', path_list=bin_paths)
            if not conf.find_program('lipo', path_list=bin_paths, mandatory=False):
                conf.find_program('lipo')
            conf.find_program('lldb', path_list=bin_paths, mandatory=False)
            conf.find_program('gdb', path_list=bin_paths, mandatory=False)
            conf.add_multiarch_toolchain(toolchain)
            pprint('GREEN', 'configured for toolchain %s' % (toolchain))


def build(bld):
    bld.platforms.append(bld.external('3rdparty.cocoa'))


def plugins(bld):
    pass


@extension('.plist')
def install_plist(self, node):
    self.bld.install_files(os.path.join(self.env.PREFIX, self.bld.optim, self.env.DEPLOY_ROOTDIR), node)


@feature('cshlib', 'cxxshlib')
@after_method('apply_link')
def set_osx_shlib_name(self):
    if 'macosx' in self.env.VALID_PLATFORMS:
        if 'plugin' in self.features:
            self.env.append_unique('LINKFLAGS', ['-install_name', os.path.join('@executable_path', '..', 'share', 'bugengine', 'plugin', self.link_task.outputs[0].name)])
        elif 'kernel' in self.features:
            self.env.append_unique('LINKFLAGS', ['-install_name', os.path.join('@executable_path', '..', 'share', 'bugengine', 'kernel', self.link_task.outputs[0].name)])
        else:
            self.env.append_unique('LINKFLAGS', ['-install_name', os.path.join('@executable_path', self.link_task.outputs[0].name)])


dsym = '${DSYMUTIL} ${DSYMUTILFLAGS} ${SRC} -o ${TGT[0].parent.parent.abspath()}'
Task.task_factory('dsym', dsym, color='BLUE')
strip = '${STRIP} ${STRIPFLAGS} -S -o ${TGT[0].abspath()} ${SRC[0].abspath()}'
Task.task_factory('strip', strip, color='BLUE')
lipo = '${LIPO} ${LIPOFLAGS} ${SRC} -create -output ${TGT[0].abspath()}'
Task.task_factory('lipo', lipo, color='BLUE')


@feature('multiarch')
def apply_multiarch_darwin(self):
    if 'darwin' in self.env.VALID_PLATFORMS:
        appname = getattr(Context.g_module, Context.APPNAME, self.bld.srcnode.name)

        features = []
        inputs = []
        for tg_name in self.use:
            task_gen = self.bld.get_tgen_by_name(tg_name)
            task_gen.post()
            if getattr(task_gen, 'link_task', None):
                inputs.append(task_gen.link_task.outputs[0])
                features += task_gen.features
        if 'plugin' in features:
            out_name = task_gen.env.cxxshlib_PATTERN % self.target
            out_path = self.env.DEPLOY_PLUGINDIR
        elif 'kernel' in features:
            out_name = task_gen.env.cxxshlib_PATTERN % self.target
            out_path = self.env.DEPLOY_KERNELDIR
        elif 'cshlib' in features or 'cxxshlib' in features:
            out_name = task_gen.env.cxxshlib_PATTERN % self.target
            out_path = self.env.DEPLOY_RUNBINDIR
        elif 'cprogram' in features or 'cxxprogram' in features:
            out_name = task_gen.env.cxxprogram_PATTERN % self.target
            out_path = self.env.DEPLOY_BINDIR
        else:
            return

        out_rootdir = os.path.join(appname+'.app.dSYM', 'Contents')
        out_rootnode = self.bld.bldnode.make_node(out_rootdir)
        out_dsymdir = out_rootnode.make_node('Resources/DWARF')

        out_node = self.bld.bldnode.make_node(out_name+'.stripped')
        out_node_dbg = self.bld.bldnode.make_node(out_name+'.dbg')
        out_node_full = self.bld.bldnode.make_node(out_name)

        self.lipo_task = self.create_task('lipo', inputs, [out_node_full])
        self.strip_task = self.create_task('strip', [out_node_full], [out_node])
        self.bld.install_as(os.path.join(self.bld.env.PREFIX, self.bld.optim, out_path, out_name), out_node, chmod=Utils.O755)

        dsymtask = getattr(self.bld, 'dsym_task', None)
        if not dsymtask:
            infoplist = self.bld.bldnode.make_node(os.path.join(out_rootdir, 'Info.plist'))
            dsymtask = self.bld.dsym_task = self.create_task('dsym', [], [infoplist])
            self.bld.install_as(os.path.join(self.bld.env.PREFIX, self.bld.optim, infoplist.path_from(self.bld.bldnode)), infoplist)

        dsymtask.set_inputs(out_node_full)
        dsymtask.set_outputs(out_dsymdir.make_node(out_name))
        self.bld.install_as(os.path.join(self.bld.env.PREFIX, self.bld.optim, appname+'.app.dSYM', 'Contents', 'Resources', 'DWARF', out_name), dsymtask.outputs[-1])

