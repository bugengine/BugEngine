import os, platform
from waflib import Options, Configure
from waflib.Logs import pprint
from waflib.Build import BuildContext, CleanContext, InstallContext, UninstallContext, ListContext
from waflib.Configure import conf


def add_build_command(toolchain, optimisation):
    c = {}
    for command in (BuildContext, CleanContext, ListContext):
        name = command.__name__.replace('Context','').lower()
        class Command(command):
            optim = optimisation
            cmd = name + ':' + toolchain + ':' + optimisation
            bugengine_variant = toolchain

            def get_variant_dir(self):
                return os.path.join(self.out_dir, self.bugengine_variant)
            variant_dir = property(get_variant_dir, None)
        c[command] = Command
    class Deploy(c[BuildContext]):
        cmd = 'deploy:%s:%s' %(toolchain, optimisation)
        bugengine_variant = toolchain

        def execute(self):
            if super(Deploy, self).execute() == "SKIP":
                return "SKIP"
            else:
                self.fun = 'deploy'
                print(self.fun)

    class Run(Deploy):
        cmd = 'run:%s:%s' %(toolchain, optimisation)

        def execute(self):
            if super(Run, self).execute() == "SKIP":
                return "SKIP"
            else:
                self.fun = 'run'
                print(self.fun)

    class Debug(Deploy):
        cmd = 'debug:%s:%s' %(toolchain, optimisation)

        def execute(self):
            if super(Debug, self).execute() == "SKIP":
                return "SKIP"
            else:
                self.fun = 'debug'
                print(self.fun)

@conf
class Platform:
    def __init__(self):
        self.directories = []

    def get_available_compilers(self, compiler_list):
        result = []
        for c in compiler_list:
            for regexp in self.SUPPORTED_TARGETS:
                if regexp.match(c.platform):
                    result.append((c, [], self))
        return result

    def add_toolchain(self, conf, compiler, sub_compilers=[], add=True):
        toolchain = '%s_%s-%s_%s-%s' % (self.NAME.lower(), compiler.arch, compiler.NAMES[0].lower(),
                                        compiler.arch_name, compiler.version)
        if sub_compilers:
            toolchain = '%s-%s-%s' % (self.NAME.lower(), compiler.NAMES[0].lower(), compiler.version)
        if add:
            conf.start_msg('  `- %s' % toolchain)
        else:
            conf.start_msg('    `- %s' % compiler.arch)
        try:
            conf.setenv(toolchain, conf.env)
            compiler.load_tools(conf, self)
            self.load_in_env(conf, compiler)
            if not sub_compilers:
                compiler.load_in_env(conf, self)
            v = conf.env
            v.ARCH_NAME = compiler.arch
            v.TOOLCHAIN=toolchain
            v.append_unique('DEFINES', ['BE_PLATFORM=platform_%s'%v.VALID_PLATFORMS[0]])
            if not add:
                v.ENV_PREFIX = compiler.arch
            if not sub_compilers:
                conf.recurse(conf.bugenginenode.abspath()+'/mak/arch/%s'%compiler.arch, once=False)

        except Exception as e:
            conf.end_msg(e, color='RED')
            conf.variant = ''
            return None
        else:
            conf.end_msg('')
            if not sub_compilers:
                conf.recurse(conf.bugenginenode.abspath()+'/mak', name='setup', once=False)
            if v.STATIC:
                v.append_unique('DEFINES', ['BE_STATIC=1'])
            conf.variant = ''
            v.TMPDIR = os.path.join(conf.bldnode.abspath(), toolchain)
            v.PREFIX = os.path.join('bld', toolchain)
            conf.variant = ''
            for c in sub_compilers:
                t = self.add_toolchain(conf, c, add=False)
                if t:
                    v.append_unique('SUB_TOOLCHAINS', [t])
            if add:
                for optim in conf.env.ALL_VARIANTS:
                    add_build_command(toolchain, optim)
                conf.env.append_unique('ALL_TOOLCHAINS', toolchain)
            return toolchain

    def add_multiarch_toolchain(self, toolchain):
        e = self.env
        e.TOOLCHAIN=toolchain
        e.append_unique('DEFINES', ['BE_PLATFORM=platform_%s'%e.VALID_PLATFORMS[0]])
        e.PREFIX = os.path.join('bld', toolchain)
        if e.STATIC:
            e.append_unique('DEFINES', ['BE_STATIC=1'])
        for optim in self.env.ALL_VARIANTS:
            add_build_command(toolchain, optim)
        self.variant = ''
        self.env.append_unique('ALL_TOOLCHAINS', toolchain)

Configure.ConfigurationContext.Platform = Platform

def options(opt):
    gr = opt.add_option_group('configure options')
    for target in opt.path.make_node('target').listdir():
        if target.endswith('.py') and not target.endswith('_build.py'):
            opt.recurse('target/%s'%target)
    for extra in opt.bugenginenode.make_node('extra').listdir():
        if os.path.isfile(os.path.join(opt.bugenginenode.abspath(), 'extra', extra, 'wscript')):
            opt.recurse(os.path.join(opt.bugenginenode.abspath(), 'extra', extra))
    gr.add_option( '--platforms',
                    action='store',
                    default='',
                    dest='platforms',
                    help='List of platform to configure for')



def configure(conf):
    conf.env.ALL_VARIANTS = ['debug', 'profile', 'final']
    conf.platforms = []
    platforms = Options.options.platforms
    platforms = platforms.split(',') if platforms else []
    for target in conf.path.make_node('target').listdir():
        if target.endswith('.py') and not target.endswith('_build.py'):
            if not platforms or target[:-3] in platforms:
                conf.recurse('target/%s'%target)
    for extra in conf.bugenginenode.make_node('extra').listdir():
        if not platforms or extra in platforms:
            if os.path.isfile(os.path.join(conf.bugenginenode.abspath(), 'extra', extra, 'wscript')):
                conf.recurse(os.path.join(conf.bugenginenode.abspath(), 'extra', extra))
    for p in conf.platforms:
        configuration_list = p.get_available_compilers(conf.compilers)
        if configuration_list:
            pprint('BLUE', ' + configuring for platform %s' % p.NAME)
            for main_toolchain, sub_toolchains, platform in configuration_list:
                platform.add_toolchain(conf, main_toolchain, sub_toolchains)
    conf.env.store('.waf_toolchains.py')


def build(bld):
    if bld.env.VALID_PLATFORMS:
        if os.path.isdir(os.path.join(bld.bugenginenode.abspath(), 'extra', bld.env.VALID_PLATFORMS[0])):
            bld.recurse(os.path.join(bld.bugenginenode.abspath(), 'extra', bld.env.VALID_PLATFORMS[0]))
        if os.path.isfile(os.path.join(bld.path.abspath(), 'target', bld.env.VALID_PLATFORMS[0]+'_build.py')):
            bld.recurse('target/%s_build'%bld.env.VALID_PLATFORMS[0])

def plugins(bld):
    if bld.env.VALID_PLATFORMS:
        extra = bld.bugenginenode.make_node('extra').make_node(bld.env.VALID_PLATFORMS[0])
        if os.path.isdir(extra.abspath()):
            bld.recurse(extra.abspath(), name='plugins')
        if os.path.isfile(os.path.join(bld.path.abspath(), 'target', bld.env.VALID_PLATFORMS[0]+'_build.py')):
            bld.recurse('target/%s_build.py'%bld.env.VALID_PLATFORMS[0], name='plugins')

from waflib import ConfigSet
try:
    env = ConfigSet.ConfigSet('.waf_toolchains.py')
    for toolchain in env.ALL_TOOLCHAINS:
        for optim in env.ALL_VARIANTS:
            add_build_command(toolchain, optim)
except:
    pass
