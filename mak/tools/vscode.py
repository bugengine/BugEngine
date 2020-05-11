from waflib import Context, Build, TaskGen, Options
import os
import sys
import json


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
                properties = {}
                for var in [
                            'Prefix', 'TmpDir', 'Toolchain', 'Deploy_BinDir', 'Deploy_RunBinDir', 'Deploy_LibDir',
                            'Deploy_IncludeDir', 'Deploy_DataDir', 'Deploy_PluginDir', 'Deploy_KernelDir', 'Deploy_RootDir'
                        ]:
                    properties[var] = env[var.upper()]
                properties['Variant'] = variant
                properties['Launcher'] = env.cxxprogram_PATTERN % self.launcher[0][0].target
                properties['Python'] = sys.executable
                if env.GDB:
                    properties['DebuggerType'] = 'cppdbg'
                    properties['DebuggerMode'] = 'gdb'
                    properties['DebuggerPath'] = env.GDB[0]
                elif env.CDB:
                    properties['DebuggerType'] = 'cppvsdbg'
                elif env.LLDB:
                    properties['DebuggerType'] = 'cppdbg'
                    properties['DebuggerMode'] = 'lldb'
                    properties['DebuggerPath'] = env.LLDB[0]
                configurations.append({
                    'name': '%s - %s' % (env_name, variant),
                    'includePath': [],
                    'defines': [],
                    'compileCommands': '${workspaceFolder}/.vscode/toolchains/%s/%s/compile_commands.json' % (env_name, variant),
                    'properties': properties
                })
                commands = []
                for g in self.groups:
                    for tg in g:
                        if not isinstance(tg, TaskGen.task_gen):
                            continue
                        if 'kernel' not in tg.features:
                            tg.post()
                            for task in tg.tasks:
                                if task.__class__.__name__ in ('cxx', 'c', 'objc', 'objcxx'):
                                    commands.append({
                                        'directory': task.get_cwd().path_from(self.path),
                                        'arguments': ['-I"%s"' % i for i in task.env.INCPATHS] + ['-D%s' % d for d in task.env.DEFINES],
                                        'file': task.inputs[0].path_from(self.path),
                                        'output': task.outputs[0].path_from(task.get_cwd())
                                    })
                with open(variant_node.make_node('compile_commands.json').abspath(), 'w') as compile_commands:
                    json.dump(commands, compile_commands, indent='  ')

        tasks_file = vscode_node.make_node('tasks.json')
        try:
            with open(tasks_file.abspath(), 'r') as document:
                tasks = json.load(document)
                tasks['tasks'] = [t for t in tasks['tasks'] if not t['label'].startswith('bugengine:')]
                try:
                    tasks['inputs'] = [i for i in tasks['inputs'] if not i['id'].startswith('bugengine-')]
                except KeyError:
                    tasks['inputs'] = []
        except IOError:
            tasks = {
                'version': '2.0.0',
                'tasks': [],
                'inputs': []
            }
        for action, command, is_default in [
            ('build', 'build:${input:bugengine-Toolchain}:${input:bugengine-Variant}', True),
            ('clean', 'clean:${input:bugengine-Toolchain}:${input:bugengine-Variant}', False),
            ('rebuild', 'rebuild:${input:bugengine-Toolchain}:${input:bugengine-Variant}', False),
            ('reconfigure', 'reconfigure', False),
            (self.cmd, self.cmd, False)
        ]:
            tasks['tasks'].append({
                'label': 'bugengine:%s' % action,
                'type': 'process',
                'command': [sys.executable],
                'args': [sys.argv[0], command] + options,
                'options': {
                    'cwd': self.srcnode.abspath()
                },
                'problemMatcher': ['$gcc', '$msCompile'],
                'group': {'kind': 'build', 'isDefault': True} if is_default else 'build'
            })
        for input in ('Toolchain', 'Variant'):
            tasks['inputs'].append({
                'id': 'bugengine-%s' % input,
                'type': 'command',
                'command': 'cpptools.activeConfigProperty',
                'args': input
            })
        with open(vscode_node.make_node('c_cpp_properties.json').abspath(), 'w') as conf_file:
            json.dump({'configurations': configurations}, conf_file, indent='  ')
        with open(tasks_file.abspath(), 'w') as document:
            json.dump(tasks, document, indent='  ')

