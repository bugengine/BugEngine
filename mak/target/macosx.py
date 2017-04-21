# set MacOS X specific options

from waflib import Configure, Options, Task, Context, Errors, Utils
from waflib.Configure import conf
from waflib.TaskGen import feature, extension, before_method, after_method
from waflib.Tools import gcc
import os
import re

@conf
class Darwin(Configure.ConfigurationContext.Platform):
    NAME = 'Darwin'
    SDK_NAME = 'Darwin'
    OS_NAME = 'darwin'
    SUPPORTED_TARGETS = (re.compile('.*-apple-darwin.*'),)

    def __init__(self, conf, sdk = None):
        Configure.ConfigurationContext.Platform.__init__(self)
        self.sdk = sdk
        self.conf = conf
        if sdk:
            self.NAME = self.__class__.NAME + sdk[0]

    def get_available_compilers(self, compiler_list):
        result = []
        compiler_sets = {}
        for c in compiler_list:
            for r in self.SUPPORTED_TARGETS:
                if r.match(c.target):
                    k = (c.__class__.__name__, c.platform, c.version)
                    try:
                        compiler_sets[k].append(c)
                    except KeyError:
                        compiler_sets[k] = [c]
                    break
        for k in sorted(compiler_sets.keys()):
            compilers = compiler_sets[k]
            try:
                compilers, sdk = self.get_best_compilers_sdk(compilers)
            except Errors.WafError:
                continue
            else:
                if len(compilers) > 1:
                    result.append((compilers[0], compilers, self.__class__(self.conf, sdk)))
                else:
                    result.append((compilers[0], [], self.__class__(self.conf, sdk)))
        return result

    def load_in_env(self, conf, compiler):
        self.CFLAGS_cshlib = ['-fPIC']
        platform = self.SDK_NAME.lower()
        conf.find_program('lipo')
        conf.find_program('dsymutil')
        conf.find_program('strip')
        conf.env.ABI = 'mach_o'
        conf.env.VALID_PLATFORMS = [platform, 'darwin', 'pc']
        conf.env.XCODE_SDKROOT = platform
        conf.env.XCODE_ABI = 'mach_o'
        conf.env.XCODE_SUPPORTEDPLATFORMS = platform
        conf.env.pymodule_PATTERN = '%s.so'

        appname = getattr(Context.g_module, Context.APPNAME, conf.srcnode.name)
        conf.env.DEPLOY_ROOTDIR = os.path.join(appname + '.app', 'Contents')
        conf.env.DEPLOY_BINDIR = os.path.join(appname + '.app', 'Contents', 'MacOS')
        conf.env.DEPLOY_RUNBINDIR = os.path.join(appname + '.app', 'Contents', 'MacOS')
        conf.env.DEPLOY_LIBDIR = 'lib'
        conf.env.DEPLOY_INCLUDEDIR = 'include'
        share = os.path.join(appname + '.app', 'Contents', 'share', 'bugengine')
        conf.env.DEPLOY_DATADIR = share
        conf.env.DEPLOY_PLUGINDIR = os.path.join(share, 'plugin')
        conf.env.DEPLOY_KERNELDIR = os.path.join(share, 'kernel')
        conf.env.append_unique('DEFINES', ['_BSD_SOURCE'])

        conf.env.MACOSX_SDK = os.path.splitext(os.path.basename(self.sdk[1]))[0]
        conf.env.XCODE_SDK_PATH = self.sdk[1]
        conf.env.SYSROOT = self.sdk[1]
        conf.env.append_unique('CFLAGS', ['-m%s-version-min=%s'%(self.OS_NAME, self.sdk[0]), '-isysroot', self.sdk[1]])
        conf.env.append_unique('CXXFLAGS', ['-m%s-version-min=%s'%(self.OS_NAME, self.sdk[0]),
                                            '-isysroot', self.sdk[1]])
        conf.env.append_unique('LINKFLAGS', ['-m%s-version-min=%s'%(self.OS_NAME, self.sdk[0]),
                                             '-isysroot', self.sdk[1], '-L%s/usr/lib'%self.sdk[1]])

    def get_best_compilers_sdk(self, compilers):
        all_archs = []
        all_paths = []
        for compiler in compilers:
            if compiler.arch not in all_archs: all_archs.append(compiler.arch)
            for sibling in compiler.siblings:
                dirname = os.path.dirname(sibling.compiler_c)
                dirname = os.path.dirname(dirname)
                dirname = os.path.dirname(dirname)
                if dirname not in all_paths: all_paths.append(dirname)

        sdk_number_min = getattr(Options.options, '%s_sdk_min' % self.OS_NAME)
        sdk_number_max = getattr(Options.options, '%s_sdk_max' % self.OS_NAME)
        sdks = []
        relpath = os.path.join('Platforms', '%s.platform'%self.SDK_NAME, 'Developer', 'SDKs')

        #for path in all_paths:
        for path in Options.options.xcode_sdks.split(','):
            for sdks_path in [os.path.join(path, relpath),
                              os.path.join(path, 'SDKs'),]:
                try:
                    sdk_list = os.listdir(sdks_path)
                except OSError:
                    continue
                else:
                    for sdk in sdk_list:
                        sdk_name,_ = os.path.splitext(sdk)
                        if sdk_name.startswith(self.SDK_NAME) and len(sdk_name) > len(self.SDK_NAME):
                            sdk_version = sdk_name[len(self.SDK_NAME):]
                            if sdk_version[-1] == 'u':
                                sdk_version = sdk_version[:-1]
                            sdks.append((sdk_version, os.path.normpath(os.path.join(sdks_path, sdk))))

        sdks.sort()
        archs = {
            '18':       { '10': 'ppc' },
            '16777234': { '0':  'ppc64' },
            '7':        { '3':  'x86' },
            '16777223': { '3':  'amd64' },
            '12':       { '6':  'armv6',
                          '9':  'armv7',
                          '11': 'armv7s' },
            '16777228': { '0':  'arm64' }
        }

        best_sdk = (None, None, [])
        for sdk, sdk_path in sdks:
            if sdk_number_max and not sdk <= sdk_number_max:
                continue
            if sdk_number_min and not sdk >= sdk_number_min:
                continue
            sdk_compilers = []
            dylib = os.path.join(sdk_path, 'usr', 'lib', 'crt1.10.5.o')
            try:
                r, out, err = compilers[0].run([self.conf.env.OTOOL, '-f', dylib])
            except Exception as e:
                print(e)
            else:
                for line in out.split('\n'):
                    line = line.split()
                    if not line:
                        continue
                    if line[0] == 'cputype':
                        cputype = line[1]
                    if line[0] == 'cpusubtype':
                        cpusubtype = line[1]
                        try:
                            a = archs[cputype][cpusubtype]
                        except KeyError:
                            print('Unknown %s arch: %s/%s in %s' % (self.OS_NAME, cputype, cpusubtype, dylib))
                        else:
                            for c in compilers:
                                if c.arch == a:
                                    sdk_compilers.append(c)
                                    break
                            else:
                                pass
                                #print('no compiler for arch %s'%a)
                if len(sdk_compilers) > len(best_sdk[2]):
                    best_sdk = (sdk, sdk_path, sdk_compilers)
                elif len(sdk_compilers) > len(best_sdk[2]) and sdk > best_sdk[0]:
                    best_sdk = (sdk, sdk_path, sdk_compilers)
        if best_sdk[2]:
            return best_sdk[2], (best_sdk[0], best_sdk[1])
        else:
            raise Errors.WafError('No SDK for compiler %s' % compilers[0].compiler_c)


class MacOSX(Darwin):
    NAME = 'MacOSX'
    SDK_NAME = 'MacOSX'
    OS_NAME = 'macosx'

    def __init__(self, conf, sdk = None):
        Darwin.__init__(self, conf, sdk)


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
    conf.platforms.append(MacOSX(conf))


def build(bld):
    bld.platforms.append(bld.external('3rdparty.cocoa'))


def plugins(bld):
    pass


@extension('.plist')
def install_plist(self, node):
    self.install_files(os.path.join(self.env.PREFIX, self.bld.optim, self.env.DEPLOY_ROOTDIR),
                       [node])


@feature('cshlib', 'cxxshlib')
@after_method('apply_link')
def set_osx_shlib_name(self):
    if 'macosx' in self.env.VALID_PLATFORMS:
        if 'plugin' in self.features:
            self.env.append_unique('LINKFLAGS', [
                    '-install_name', os.path.join('@rpath', '..', 'share', 'bugengine', 'plugin', self.link_task.outputs[0].name)
                ])
        elif 'kernel' in self.features:
            self.env.append_unique('LINKFLAGS', [
                    '-install_name', os.path.join('@rpath', '..', 'share', 'bugengine', 'plugin', self.link_task.outputs[0].name)
                ])
        else:
            self.env.append_unique('LINKFLAGS', [
                    '-Wl,-rpath,@loader_path/',
                    '-install_name', os.path.join('@rpath', self.link_task.outputs[0].name)
                ])


@feature('cprogram', 'cxxprogram')
@after_method('apply_link')
def set_osx_program_name(self):
    if 'macosx' in self.env.VALID_PLATFORMS:
        self.env.append_unique('LINKFLAGS', [
                '-Wl,-rpath,@loader_path/',
            ])


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

        out_node = self.make_bld_node('.bin', None, out_name+'.stripped')
        out_node_dbg = self.make_bld_node('.bin', None, out_name+'.dbg')
        out_node_full = self.make_bld_node('.bin', None, out_name)

        self.lipo_task = self.create_task('lipo', inputs, [out_node_full])
        self.strip_task = self.create_task('strip', [out_node_full], [out_node])
        self.install_as(os.path.join(self.bld.env.PREFIX, self.bld.optim, out_path, out_name),
                        out_node,
                        chmod=Utils.O755)

        dsymtask = getattr(self.bld, 'dsym_task', None)
        if not dsymtask:
            infoplist = self.bld.bldnode.make_node(os.path.join(out_rootdir, 'Info.plist'))
            dsymtask = self.bld.dsym_task = self.create_task('dsym', [], [infoplist])
            self.install_as(os.path.join(self.bld.env.PREFIX, self.bld.optim,
                                         infoplist.path_from(self.bld.bldnode)),
                            infoplist)

        dsymtask.set_inputs(out_node_full)
        dsymtask.set_outputs(out_dsymdir.make_node(out_name))
        self.install_as(os.path.join(self.bld.env.PREFIX, self.bld.optim, appname+'.app.dSYM',
                                     'Contents', 'Resources', 'DWARF', out_name),
                        dsymtask.outputs[-1])

