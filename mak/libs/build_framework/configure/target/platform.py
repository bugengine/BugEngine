import os
import platform
from waflib import Options, Configure
from waflib.Logs import pprint
from waflib.Build import BuildContext, CleanContext, InstallContext, UninstallContext, ListContext
from waflib.Configure import conf


class Platform:
    def __init__(self):
        self.directories = []

    def get_available_compilers(self, configuration_context, compiler_list):
        result = []
        for c in compiler_list:
            for regexp in self.SUPPORTED_TARGETS:
                if regexp.match(c.platform):
                    result.append((c, [], self))
        return result

    def platform_name(self, compiler):
        return self.NAME.lower()

    def add_toolchain(self, conf, compiler, sub_compilers=[], add=True):
        toolchain = '%s_%s-%s_%s-%s' % (
            self.platform_name(compiler), compiler.arch, compiler.NAMES[0].lower(), compiler.arch_name, compiler.version
        )
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
                v.ENV_PREFIX = compiler.arch + '/%s'
                v.SUBARCH = True
            else:
                v.ENV_PREFIX = '%s'
                v.SUBARCH = False
            if not sub_compilers:
                conf.recurse('../arch/arch.py', once=False)
        except Exception as e:
            conf.end_msg(e, color='RED')
            conf.variant = ''
            return None
        else:
            conf.end_msg(conf.env.COMPILER_ABI)
            #if not sub_compilers:
            #    conf.recurse(conf.bugenginenode.abspath(), name='setup', once=False)
            if v.STATIC:
                v.append_unique('DEFINES', ['BE_STATIC=1'])
            v.TMPDIR = os.path.join(conf.bldnode.abspath(), toolchain)
            v.PREFIX = os.path.join('bld', toolchain)
            conf.variant = ''
            for c in sub_compilers:
                t = self.add_toolchain(conf, c, add=False)
                if t:
                    v.append_unique('SUB_TOOLCHAINS', [t])
            if add:
                conf.env.append_unique('ALL_TOOLCHAINS', toolchain)
            return toolchain

    def add_multiarch_toolchain(self, toolchain):
        e = self.env
        e.TOOLCHAIN = toolchain
        e.append_unique('DEFINES', ['BE_PLATFORM=platform_%s' % e.VALID_PLATFORMS[0]])
        e.PREFIX = os.path.join('bld', toolchain)
        if e.STATIC:
            e.append_unique('DEFINES', ['BE_STATIC=1'])
        self.variant = ''
        self.env.append_unique('ALL_TOOLCHAINS', toolchain)


Configure.ConfigurationContext.Platform = Platform


def configure(configuration_context):
    pass