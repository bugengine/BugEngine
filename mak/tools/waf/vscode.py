from waflib import Context, Build, TaskGen, Logs
import os, sys


def unique(seq):
    seen = set()
    seen_add = seen.add
    return [ x for x in seq if x not in seen and not seen_add(x)]

def gather_includes_defines(task_gen):
    defines = getattr(task_gen, 'defines', [])
    includes = getattr(task_gen, 'includes', [])
    seen = []
    use = getattr(task_gen, 'use', [])[:]
    while use:
        name = use.pop()
        if name not in seen:
            try:
                t = task_gen.bld.get_tgen_by_name(name)
            except:
                pass
            else:
                use = use + getattr(t, 'use', [])
                includes = includes + getattr(t, 'includes ', [])
                defines = defines + getattr(t, 'defines ', [])
    return unique(includes), unique(defines)

def path_from(path, bld):
    if isinstance(path, str):
        return path
    else:
        return path.abspath()

class vscode(Build.BuildContext):
    "creates projects for Visual Studio Code"
    cmd = 'vscode'
    fun = 'build'
    optim = 'debug'

    def execute(self):
        """
        Entry point
        """
        self.restore()
        if not self.all_envs:
            self.load_envs()
        self.env.PROJECTS=[self.__class__.cmd]

        self.env.VARIANT = '${Variant}'
        self.env.TOOLCHAIN = '${Toolchain}'
        self.env.PREFIX = '${Prefix}'
        self.env.TMPDIR = '${TmpDir}'
        self.env.DEPLOY_ROOTDIR = '${Deploy_RootDir}'
        self.env.DEPLOY_BINDIR = '${Deploy_BinDir}'
        self.env.DEPLOY_RUNBINDIR = '${Deploy_RunBinDir}'
        self.env.DEPLOY_LIBDIR = '${Deploy_LibDir}'
        self.env.DEPLOY_INCLUDEDIR = '${Deploy_IncludeDir}'
        self.env.DEPLOY_DATADIR = '${Deploy_DataDir}'
        self.env.DEPLOY_PLUGINDIR = '${Deploy_PluginDir}'
        self.env.DEPLOY_KERNELDIR = '${Deploy_KernelDir}'
        self.features = ['GUI']

        self.recurse([self.run_dir])

        appname = getattr(Context.g_module, Context.APPNAME, self.srcnode.name)

        workspace_node = self.srcnode.make_node('%s.code-workspace' % appname)
        extra_node = self.bugenginenode.make_node('extra')

        with open(workspace_node.abspath(), 'w') as workspace:
            workspace.write('{\n  "folders": [\n')
            for g in self.groups:
                for tg in g:
                    if not isinstance(tg, TaskGen.task_gen):
                        continue
                    if not 'kernel' in tg.features:
                        tg.post()
                    includes, defines = gather_includes_defines(tg)
                    for node in tg.source_nodes:
                        name = tg.name
                        path = node.path_from(self.srcnode)
                        vscode_node = node.make_node('.vscode')
                        vscode_node.mkdir()
                        settings_node = vscode_node.make_node('c_cpp_properties.json')
                        with open(settings_node.abspath(), 'w') as settings:
                            settings.write('{\n  "configurations": [\n')
                            for toolchain in tg.bld.env.ALL_TOOLCHAINS:
                                env = tg.bld.all_envs[toolchain]
                                if env.SUB_TOOLCHAINS:
                                    sub_env = tg.bld.all_envs[env.SUB_TOOLCHAINS[0]]
                                else:
                                    sub_env = env
                                for variant in tg.bld.env.ALL_VARIANTS:
                                    all_defines = ('"%s"' % d for d in sub_env.DEFINES + defines)
                                    all_includes = ('"%s"' % path_from(p, tg.bld) for p in (includes + sub_env.INCLUDES + sub_env.SYSTEM_INCLUDES + [os.path.join(sub_env.SYSROOT or '', 'usr', 'include')]))
                                    settings.write('    {\n'
                                                   '      "name": "%s:%s",\n'
                                                   '      "includePath": [\n'
                                                   '        %s\n'
                                                   '      ],\n'
                                                   '      "defines": [\n'
                                                   '        %s\n'
                                                   '      ]\n'
                                                   '    },\n' % (toolchain, variant,
                                                                 ',\n        '.join(all_includes),
                                                                 ',\n        '.join(all_defines)))
                            settings.write('  ]\n}\n')
                        if node.is_child_of(extra_node):
                            while node.parent != extra_node:
                                node = node.parent
                            name += '[%s]'%node
                        workspace.write('    {\n'
                                        '        "name": "%s",\n'
                                        '        "path": "%s"\n'
                                        '    },\n' % (name, path))
            workspace.write('  ]\n}n')

