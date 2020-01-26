from waflib import Context, Build, TaskGen
import os
import sys


def unique(seq):
    seen = set()
    seen_add = seen.add
    return [x for x in seq if x not in seen and not seen_add(x)]


def gather_includes_defines(task_gen):
    defines = getattr(task_gen, 'defines', []) + getattr(task_gen, 'export_defines',
                                                         []) + getattr(task_gen, 'extra_defines', [])
    includes = getattr(task_gen, 'includes', []) + getattr(task_gen, 'export_includes',
                                                           []) + getattr(task_gen, 'extra_includes', [])
    seen = set([])
    use = getattr(task_gen, 'use', []) + getattr(task_gen, 'private_use', [])
    while use:
        name = use.pop()
        if name not in seen:
            seen.add(name)
            try:
                t = task_gen.bld.get_tgen_by_name(name)
            except:
                pass
            else:
                use = use + getattr(t, 'use', [])
                includes = includes + getattr(t, 'includes',
                                              []) + getattr(t, 'export_includes',
                                                            []) + getattr(task_gen, 'extra_includes', [])
                defines = defines + getattr(t, 'defines', []) + getattr(t, 'export_defines',
                                                                        []) + getattr(task_gen, 'extra_defines', [])
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
    variant = 'projects/vscode'

    SETTINGS = '  {\n' \
               '    "editor.formatOnSave": true,\n' \
               '    "editor.formatOnType": true,\n' \
               '    "python.formatting.provider": "autopep8",\n' \
               '    "python.linting.mypyArgs": [\n' \
               '      "--follow-imports=silent"\n' \
               '    ],\n' \
               '    "python.linting.mypyEnabled": true,\n' \
               '    "python.linting.pylintEnabled": false,\n' \
               '    "python.formatting.autopep8Path": "yapf",\n' \
               '    "python.formatting.autopep8Args": [\n' \
               '      "--style=%(bugenginepath)s/setup.cfg"\n' \
               '    ]\n' \
               '  }\n'

    def execute(self):
        """
        Entry point
        """
        self.restore()
        if not self.all_envs:
            self.load_envs()
        self.env.PROJECTS = [self.__class__.cmd]

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
        self.write_workspace()

    def write_workspace(self):
        appname = getattr(Context.g_module, Context.APPNAME, self.srcnode.name)

        workspace_node = self.srcnode.make_node('%s.code-workspace' % appname)
        extra_node = self.bugenginenode.make_node('extra')

        with open(workspace_node.abspath(), 'w') as workspace:
            workspace.write(
                '{\n'
                '  "folders": [\n'
                '    {\n'
                '      "path": ".",\n'
                '      "name": "%s"\n'
                '    }\n'
                '  ],\n' % appname
            )
            workspace.write(
                '  "settings": %s\n'
                '}\n' % (self.SETTINGS % {
                    'bugenginepath': self.bugenginenode.abspath()
                })
            )


class vscode_folders(vscode):
    "creates projects for Visual Studio Code"
    cmd = 'vscode_folders'
    fun = 'build'
    optim = 'debug'

    def write_workspace(self):
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
                        if not node.isdir():
                            continue
                        name = tg.name
                        path = node.path_from(self.srcnode)
                        vscode_node = node.make_node('.vscode')
                        vscode_node.mkdir()
                        settings_node = vscode_node.make_node('c_cpp_properties.json')
                        with open(settings_node.abspath(), 'w') as settings:
                            configs = []
                            for toolchain in tg.bld.env.ALL_TOOLCHAINS:
                                env = tg.bld.all_envs[toolchain]
                                if env.SUB_TOOLCHAINS:
                                    sub_env = tg.bld.all_envs[env.SUB_TOOLCHAINS[0]]
                                else:
                                    sub_env = env
                                for variant in tg.bld.env.ALL_VARIANTS:
                                    all_defines = ('"%s"' % d for d in sub_env.DEFINES + defines)
                                    all_includes = (
                                        '"%s"' % path_from(p, tg.bld) for p in (
                                            includes + sub_env.INCLUDES + sub_env.SYSTEM_INCLUDES +
                                            [os.path.join(sub_env.SYSROOT or '', 'usr', 'include')]
                                        )
                                    )
                                    configs.append(
                                        '    {\n'
                                        '      "name": "%s:%s",\n'
                                        '      "includePath": [\n'
                                        '        %s\n'
                                        '      ],\n'
                                        '      "defines": [\n'
                                        '        %s\n'
                                        '      ]\n'
                                        '    }' % (
                                            toolchain, variant, ',\n        '.join(all_includes),
                                            ',\n        '.join(all_defines)
                                        )
                                    )
                            settings.write('{\n  "configurations": [\n%s\n  ]\n}\n' % (',\n'.join(configs)))
                        if node.is_child_of(extra_node):
                            while node.parent != extra_node:
                                node = node.parent
                            name += '[%s]' % node.name
                        workspace.write(
                            '    {\n'
                            '        "name": "%s",\n'
                            '        "path": "%s"\n'
                            '    },\n' % (name, path)
                        )

                    if 'Makefile' in tg.features:
                        root = tg.source_nodes[0]
                        vscode_node = root.make_node('.vscode')
                        tasks = []
                        options = [a for a in sys.argv if a[0] == '-']
                        for env_name in self.env.ALL_TOOLCHAINS:
                            bld_env = self.all_envs[env_name]
                            if bld_env.SUB_TOOLCHAINS:
                                env = self.all_envs[bld_env.SUB_TOOLCHAINS[0]]
                            else:
                                env = bld_env
                            for variant in self.env.ALL_VARIANTS:
                                tasks.append(
                                    '    {\n'
                                    '      "label": "build:%(toolchain)s:%(variant)s",\n'
                                    '      "type": "process",\n'
                                    '      "command": ["%(python)s"],\n'
                                    '      "args": ["%(waf)s", %(cl)s],\n'
                                    '      "options":{\n'
                                    '        "cwd": "%(pwd)s"\n'
                                    '      },\n'
                                    '      "problemMatcher": [\n'
                                    '        "$gcc",\n'
                                    '        "$msCompile"\n'
                                    '      ],\n'
                                    '      "group": {\n'
                                    '        "kind": "build",\n'
                                    '        "isDefault": true\n'
                                    '      }\n'
                                    '    }\n' % {
                                        'python':
                                            sys.executable,
                                        'waf':
                                            sys.argv[0],
                                        'toolchain':
                                            env_name,
                                        'variant':
                                            variant,
                                        'cl':
                                            ', '.join(
                                                '"%s"' % o for o in options + ['build:%s:%s' % (env_name, variant)]
                                            ),
                                        'pwd':
                                            tg.bld.srcnode.abspath()
                                    }
                                )
                        with open(vscode_node.make_node('tasks.json').abspath(), 'w') as task_file:
                            task_file.write('{\n  "version":"2.0.0",\n  "tasks": [\n')
                            task_file.write(',\n'.join(tasks))
                            task_file.write('\n  ]\n}\n')
            workspace.write(
                '  "settings": %s\n'
                '}\n' % (self.SETTINGS % {
                    'bugenginepath': self.bugenginenode.abspath()
                })
            )
