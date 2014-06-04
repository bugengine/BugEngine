import os, platform
from waflib.Logs import pprint
from waflib.Build import BuildContext, CleanContext, InstallContext, UninstallContext, ListContext
from waflib.Configure import conf


def add_build_command(toolchain, optimisation):
    for command in (BuildContext, CleanContext, InstallContext, UninstallContext, ListContext):
        name = command.__name__.replace('Context','').lower()
        class Command(command):
            optim = optimisation
            cmd = name + ':' + toolchain + ':' + optimisation
            variant = toolchain

            def get_variant_dir(self):
                return os.path.join(self.out_dir, self.variant, self.optim)
            variant_dir = property(get_variant_dir, None)

@conf
def add_toolchain(self, platform, compiler_arch, compiler, version, arch, add=True):
    toolchain = '%s-%s-%s-%s' % (platform, compiler_arch, compiler, version)
    e = self.env
    e.TOOLCHAIN=toolchain
    self.recurse(self.bugenginenode.abspath()+'/mak/arch/%s'%arch, once=False)
    self.recurse(self.bugenginenode.abspath()+'/mak', name='setup', once=False)
    e.DEFINES.append('BE_PLATFORM=platform_%s'%e.VALID_PLATFORMS[0])
    if e.STATIC:
        e.DEFINES.append('BE_STATIC=1')
    if add:
        e.PREFIX = os.path.join('build', toolchain)
        for optim in self.env.ALL_VARIANTS:
            add_build_command(toolchain, optim)
        self.variant = ''
        self.env.append_unique('ALL_TOOLCHAINS', toolchain)
        self.env.append_unique('ALL_PLATFORMS', [p for p in e.VALID_PLATFORMS if p not in self.env.ALL_PLATFORMS])
        self.env.append_unique('ALL_ARCHITECTURES', [a for a in e.VALID_ARCHITECTURES if a not in self.env.ALL_ARCHITECTURES])

@conf
def add_multiarch_toolchain(self, toolchain):
    e = self.env
    e.TOOLCHAIN=toolchain
    e.DEFINES.append('BE_PLATFORM=platform_%s'%e.VALID_PLATFORMS[0])
    e.PREFIX = os.path.join('build', toolchain)
    if e.STATIC:
        e.DEFINES.append('BE_STATIC=1')
    for optim in self.env.ALL_VARIANTS:
        add_build_command(toolchain, optim)
    self.variant = ''
    self.env.append_unique('ALL_TOOLCHAINS', toolchain)
    self.env.append_unique('ALL_PLATFORMS', [p for p in e.VALID_PLATFORMS if p not in self.env.ALL_PLATFORMS])
    self.env.append_unique('ALL_ARCHITECTURES', [a for a in e.VALID_ARCHITECTURES if a not in self.env.ALL_ARCHITECTURES])


def options(opt):
    gr = opt.add_option_group('configure options')
    for extra in opt.bugenginenode.make_node('extra').listdir():
        if os.path.isfile(os.path.join(opt.bugenginenode.abspath(), 'extra', extra, 'wscript')):
            opt.recurse(os.path.join(opt.bugenginenode.abspath(), 'extra', extra))
    for target in opt.path.make_node('target').listdir():
        if target.endswith('.py'):
            opt.recurse('target/%s'%target)
    gr.add_option( '--platforms',
                    action='store',
                    default='',
                    dest='platforms',
                    help='List of platform to configure for')



def configure(conf):
    conf.env.ALL_VARIANTS = ['debug', 'profile', 'final']
    from waflib import Options
    platforms = Options.options.platforms
    platforms = platforms.split(',') if platforms else []
    for extra in conf.bugenginenode.make_node('extra').listdir():
        if not platforms or extra in platforms:
            if os.path.isfile(os.path.join(conf.bugenginenode.abspath(), 'extra', extra, 'wscript')):
                pprint('BLUE', '_'*40)
                pprint('BLUE', '| '+('configure package %s'%extra).ljust(36)+' |')
                conf.recurse(os.path.join(conf.bugenginenode.abspath(), 'extra', extra))
    for target in conf.path.make_node('target').listdir():
        if not platforms or target[:-3] in platforms:
            pprint('BLUE', '_'*40)
            pprint('BLUE', '| '+('configure for target %s'%target).ljust(36)+' |')
            conf.recurse('target/%s'%target)
    conf.env.VALID_PLATFORMS = conf.env.ALL_PLATFORMS
    conf.env.VALID_RCHITECTURES = conf.env.ALL_ARCHITECTURES
    conf.env.store('.build/be_toolchains.py')


def build(bld):
    if bld.env.VALID_PLATFORMS:
        if os.path.isdir(os.path.join(bld.bugenginenode.abspath(), 'extra', bld.env.VALID_PLATFORMS[0])):
            bld.recurse(os.path.join(bld.bugenginenode.abspath(), 'extra', bld.env.VALID_PLATFORMS[0]))
        if os.path.isfile(os.path.join(bld.path.abspath(), 'target', bld.env.VALID_PLATFORMS[0]+'.py')):
            bld.recurse('target/%s'%bld.env.VALID_PLATFORMS[0])

def plugins(bld):
    if bld.env.VALID_PLATFORMS:
        extra = bld.bugenginenode.make_node('extra').make_node(bld.env.VALID_PLATFORMS[0])
        if os.path.isdir(extra.abspath()):
            bld.recurse(extra.abspath(), name='plugins')
        if os.path.isfile(os.path.join(bld.path.abspath(), 'target', bld.env.VALID_PLATFORMS[0]+'.py')):
            bld.recurse('target/%s'%bld.env.VALID_PLATFORMS[0], name='plugins')

from waflib import ConfigSet
try:
    env = ConfigSet.ConfigSet('.build/be_toolchains.py')
    for toolchain in env.ALL_TOOLCHAINS:
        for optim in env.ALL_VARIANTS:
            add_build_command(toolchain, optim)
except:
    pass
