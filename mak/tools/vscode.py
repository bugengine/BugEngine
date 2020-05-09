from waflib import Context, Build, TaskGen, Options
import os
import sys


class vscode(Build.BuildContext):
    "creates projects for Visual Studio Code"
    cmd = 'vscode'
    fun = 'build'
    optim = 'debug'
    variant = 'projects/vscode'

    SETTINGS = '  {\n' \
               '    "editor.formatOnSave": true,\n' \
               '    "editor.formatOnType": true,\n' \
               '    "python.formatting.provider": "yapf",\n' \
               '    "python.linting.mypyArgs": [\n' \
               '      "--follow-imports=silent"\n' \
               '    ],\n' \
               '    "python.linting.mypyEnabled": true,\n' \
               '    "python.linting.pylintEnabled": false,\n' \
               '    "python.formatting.yapfArgs": [\n' \
               '      "--style=%(bugenginepath)s/setup.cfg"\n' \
               '    ],\n' \
               '    "files.exclude": {\n' \
               '      "**/.git": true,\n' \
               '      "**/.svn": true,\n' \
               '      "**/.hg": true,\n' \
               '      "**/CVS": true,\n' \
               '      "**/.DS_Store": true,\n' \
               '      "**/__pycache__": true,\n' \
               '      "**/*.pyc": true,\n' \
               '      "bld/.waf": true,\n' \
               '      "**/.clangd": true,\n' \
               '      "**/.mypy_cache": true,\n' \
               '      "%(bugenginepath)s/mak/host": true\n' \
               '    },\n' \
               '    "clangd.arguments": [\n' \
               '      "--header-insertion=never"\n' \
               '    ]\n' \
               '  }\n'

    def execute(self):
        """
        Entry point
        """
        self.restore()
        Options.options.nomaster = True
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
                    'bugenginepath': self.bugenginenode.path_from(self.path)
                })
            )
        options = [a for a in sys.argv if a[0] == '-']
        tasks = []
        configurations = []
        vscode_node = self.srcnode.make_node('.vscode')
        vscode_node.mkdir()
        for env_name in self.env.ALL_TOOLCHAINS:
            toolchain_node = vscode_node.make_node('toolchains').make_node(env_name)
            bld_env = self.all_envs[env_name]
            if bld_env.SUB_TOOLCHAINS:
                env = self.all_envs[bld_env.SUB_TOOLCHAINS[0]]
            else:
                env = bld_env
            for variant in self.env.ALL_VARIANTS:
                variant_node = toolchain_node.make_node(variant)
                variant_node.mkdir()
                configurations.append(
                    '    {\n'
                    '      "name": "%(toolchain)s:%(variant)s",\n'
                    '      "includePath": [],\n'
                    '      "defines": [],\n'
                    '      "compileCommands": "${workspaceFolder}/.vscode/toolchains/%(toolchain)s/%(variant)s/compile_commands.json"\n'
                    '    }' % {
                        'toolchain': env_name,
                        'variant': variant
                    }
                )
                commands = []
                for g in self.groups:
                    for tg in g:
                        if not isinstance(tg, TaskGen.task_gen):
                            continue
                        if 'kernel' not in tg.features:
                            tg.post()
                            for task in tg.tasks:
                                if task.__class__.__name__ in ('cxx', 'c', 'objc', 'objcxx'):
                                    commands.append('\t{\n'
                                                    '\t\t"directory": "%s",\n'
                                                    '\t\t"arguments": [%s],\n'
                                                    '\t\t"file": "%s",\n'
                                                    '\t\t"output": "%s"\n'
                                                    '\t}' % (task.get_cwd().path_from(self.path).replace('\\', '/'),
                                                            ", ".join(['"-I%s"' % i.replace('\\', '/') for i in task.env.INCPATHS] + ['"-D%s"' % d for d in task.env.DEFINES]),
                                                                task.inputs[0].path_from(self.path).replace('\\', '/'),
                                                                task.outputs[0].path_from(task.get_cwd()).replace('\\', '/')))
                with open(variant_node.make_node('compile_commands.json').abspath(), 'w') as compile_commands:
                    compile_commands.write('[\n')
                    compile_commands.write(',\n'.join(commands))
                    compile_commands.write('\n]')


        for action, command, is_default in [
            ('build', 'build:${command:cpptools.activeConfigName}',
             True), ('clean', 'clean:${command:cpptools.activeConfigName}', False),
            ('rebuild', 'rebuild:${command:cpptools.activeConfigName}', False), ('reconfigure', 'reconfigure',
                                                                                 False), (self.cmd, self.cmd, False)
        ]:
            tasks.append(
                '    {\n'
                '      "label": "%(action)s",\n'
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
                '      "group": %(group)s\n'
                '    }' % {
                    'action': action,
                    'group': '{\n        "kind": "build",\n        "isDefault": true\n      }' if is_default else '"build"',
                    'python': sys.executable.replace('\\', '/'),
                    'waf': sys.argv[0].replace('\\', '/'),
                    'toolchain': env_name,
                    'variant': variant,
                    'cl': ', '.join('"%s"' % o for o in options + [command]),
                    'pwd': self.srcnode.abspath().replace('\\', '/')
                }
            )
        with open(vscode_node.make_node('c_cpp_properties.json').abspath(), 'w') as conf_file:
            conf_file.write('{\n  "configurations": [\n')
            conf_file.write(',\n'.join(configurations))
            conf_file.write('\n  ]\n}\n')
        with open(vscode_node.make_node('tasks.json').abspath(), 'w') as task_file:
            task_file.write('{\n  "version":"2.0.0",\n  "tasks": [\n')
            task_file.write(',\n'.join(tasks))
            task_file.write('\n  ]\n}\n')

