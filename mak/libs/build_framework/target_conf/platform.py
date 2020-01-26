import os
import platform
from waflib import Options, Configure
from waflib.Logs import pprint
from waflib.Build import BuildContext, CleanContext, InstallContext, UninstallContext, ListContext
from waflib.Configure import conf
from . import arch


def add_build_command(toolchain, optimisation):
    c = {}
    for command in (BuildContext, CleanContext, ListContext):
        name = command.__name__.replace('Context', '').lower()

        class Command(command):
            optim = optimisation
            cmd = name + ':' + toolchain + ':' + optimisation
            bugengine_variant = toolchain
            variant = os.path.join(toolchain, optimisation)

        c[command] = Command

    class Deploy(c[BuildContext]):
        cmd = 'deploy:%s:%s' % (toolchain, optimisation)
        bugengine_variant = toolchain
        variant = os.path.join(toolchain, optimisation)

        def execute(self):
            if super(Deploy, self).execute() == "SKIP":
                return "SKIP"
            else:
                self.fun = 'deploy'
                self.recurse(self.bugenginenode.abspath())

    class Run(Deploy):
        cmd = 'run:%s:%s' % (toolchain, optimisation)

        def execute(self):
            if super(Run, self).execute() == "SKIP":
                return "SKIP"
            else:
                self.fun = 'run'
                self.recurse(self.bugenginenode.abspath())

    class Debug(Deploy):
        cmd = 'debug:%s:%s' % (toolchain, optimisation)

        def execute(self):
            if super(Debug, self).execute() == "SKIP":
                return "SKIP"
            else:
                self.fun = 'debug'
                self.recurse(self.bugenginenode.abspath())


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
        toolchain = '%s_%s-%s_%s-%s' % (self.NAME.lower(), compiler.arch, compiler.NAMES[0].lower(), compiler.arch_name,
                                        compiler.version)
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
            v.TOOLCHAIN = toolchain
            v.append_unique('DEFINES', ['BE_PLATFORM=platform_%s' % v.VALID_PLATFORMS[0]])
            if not add:
                v.ENV_PREFIX = compiler.arch
            if not sub_compilers:
                arch.configure(conf, compiler.arch)
                self.add_kernel_toolchains(conf)
        except Exception as e:
            conf.end_msg(e, color='RED')
            conf.variant = ''
            return None
        else:
            conf.end_msg(' ')
            if not sub_compilers:
                conf.recurse(conf.bugenginenode.abspath(), name='setup', once=False)
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

    def add_kernel_toolchains(self, conf):
        conf.env.KERNEL_TOOLCHAINS += [('cpu', conf.env.TOOLCHAIN)]

    def add_multiarch_toolchain(self, toolchain):
        e = self.env
        e.TOOLCHAIN = toolchain
        e.append_unique('DEFINES', ['BE_PLATFORM=platform_%s' % e.VALID_PLATFORMS[0]])
        e.PREFIX = os.path.join('bld', toolchain)
        if e.STATIC:
            e.append_unique('DEFINES', ['BE_STATIC=1'])
        for optim in self.env.ALL_VARIANTS:
            add_build_command(toolchain, optim)
        self.variant = ''
        self.env.append_unique('ALL_TOOLCHAINS', toolchain)


from waflib import ConfigSet
try:
    env = ConfigSet.ConfigSet('.waf_toolchains.py')
    for toolchain in env.ALL_TOOLCHAINS:
        for optim in env.ALL_VARIANTS:
            add_build_command(toolchain, optim)
except:
    pass
