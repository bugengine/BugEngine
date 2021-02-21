from waflib import Context, Build, TaskGen, Options, Utils
from waflib.TaskGen import feature
import os
import sys
import json
from json_minify import json_minify


class vscode(Build.BuildContext):
    "creates projects for Visual Studio Code"
    cmd = 'vscode'
    fun = 'build'
    optim = 'debug'
    bugengine_toolchain = 'projects'
    bugengine_variant = 'projects.setup'
    variant = 'projects/vscode'

    SETTINGS = '  {\n' \
               '    "editor.formatOnSave": true,\n' \
               '    "editor.formatOnType": true,\n' \
               '    "python.linting.mypyArgs": [\n' \
               '      "--follow-imports=silent"\n' \
               '    ],\n' \
               '    "python.linting.mypyEnabled": true,\n' \
               '    "python.linting.pylintEnabled": false,\n' \
               '    "python.formatting.yapfArgs": [\n' \
               '      "--style=%(bugenginepath)s/setup.cfg"\n' \
               '    ],\n' \
               '    "python.formatting.autopep8Path": "yapf",\n' \
               '    "python.formatting.provider": "autopep8",\n' \
               '    "python.autoComplete.extraPaths": [\n' \
               '      "%(bugenginepath)s/mak/libs"\n' \
               '    ],\n' \
               '    "files.exclude": {\n' \
               '      "**/.git": true,\n' \
               '      "**/.svn": true,\n' \
               '      "**/.hg": true,\n' \
               '      "**/CVS": true,\n' \
               '      "**/.DS_Store": true,\n' \
               '      "**/__pycache__": true,\n' \
               '      "**/*.pyc": true,\n' \
               '      "bld/.waf/*-*-*": true,\n' \
               '      "bld/.waf/*.*": true,\n' \
               '      "**/.clangd": true,\n' \
               '      "**/.mypy_cache": true,\n' \
               '    },\n' \
               '    "files.watcherExclude": {\n' \
               '      "**/.git/*": true,\n' \
               '      "**/bld/**": true\n' \
               '    },\n' \
               '    "clangd.arguments": [\n' \
               '      "--header-insertion=never"\n' \
               '    ]\n' \
               '  }\n'

    def execute(self):
        """
        Entry point
        """
        if self.schedule_setup():
            return "SKIP"

        self.restore()
        Options.options.nomaster = True
        if not self.all_envs:
            self.load_envs()
        self.variant = self.__class__.bugengine_variant
        self.env.PROJECTS = [self.__class__.cmd]

        self.env.VARIANT = '${input:bugengine-Variant}'
        self.env.TOOLCHAIN = '${input:bugengine-Toolchain}'
        self.env.PREFIX = '${input:bugengine-Prefix}'
        self.env.TMPDIR = '${input:bugengine-TmpDir}'
        self.env.DEPLOY_ROOTDIR = '${input:bugengine-Deploy_RootDir}'
        self.env.DEPLOY_BINDIR = '${input:bugengine-Deploy_BinDir}'
        self.env.DEPLOY_RUNBINDIR = '${input:bugengine-Deploy_RunBinDir}'
        self.env.DEPLOY_LIBDIR = '${input:bugengine-Deploy_LibDir}'
        self.env.DEPLOY_INCLUDEDIR = '${input:bugengine-Deploy_IncludeDir}'
        self.env.DEPLOY_DATADIR = '${input:bugengine-Deploy_DataDir}'
        self.env.DEPLOY_PLUGINDIR = '${input:bugengine-Deploy_PluginDir}'
        self.env.DEPLOY_KERNELDIR = '${input:bugengine-Deploy_KernelDir}'
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
                '  "extensions": {\n'
                '    "recommendations": [\n'
                '      "ms-vscode.cpptools",\n'
                '      "ms-python.python"\n'
                '    ]\n'
                '  },\n'
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
                    properties[var] = bld_env[var.upper()]
                properties['Variant'] = variant
                properties['Launcher'] = env.cxxprogram_PATTERN % self.launcher.target
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
                else:
                    properties['DebuggerType'] = 'cppdbg'
                    properties['DebuggerMode'] = 'gdb'
                    properties['DebuggerPath'] = '/usr/bin/gdb'
                commands = []
                include_paths = []
                defines = []
                for g in self.groups:
                    for tg in g:
                        if not isinstance(tg, TaskGen.task_gen):
                            continue
                        tg.post()
                        include_paths += getattr(tg, 'includes', [])
                        include_paths += getattr(tg, 'export_includes', [])
                        include_paths += getattr(tg, 'extra_includes', [])
                        defines += getattr(tg, 'defines', [])
                        defines += getattr(tg, 'export_defines', [])
                        defines += getattr(tg, 'extra_defines', [])
                        for task in tg.tasks:
                            if task.__class__.__name__ in ('cxx', 'c', 'objc', 'objcxx'):
                                commands.append(
                                    {
                                        'directory':
                                            task.get_cwd().path_from(self.path),
                                        'arguments':
                                            env.CXX + ['-I"%s"' % i for i in env.INCLUDES + task.env.INCPATHS] +
                                            ['-D%s' % d for d in task.env.DEFINES + env.DEFINES],
                                        'file':
                                            task.inputs[0].path_from(self.path),
                                        'output':
                                            task.outputs[0].path_from(task.get_cwd())
                                    }
                                )
                with open(variant_node.make_node('compile_commands.json').abspath(), 'w') as compile_commands:
                    json.dump(commands, compile_commands, indent=2)
                seen = set([self.srcnode, self.bldnode])
                configurations.append(
                    {
                        'name':
                            '%s - %s' % (env_name, variant),
                        'includePath':
                            [
                                os.path.join('${workspaceFolder}', i.path_from(self.srcnode)) for i in include_paths
                                if i not in seen and not seen.add(i)
                            ],
                        'defines': [d for d in defines if d not in seen and not seen.add(d)],
                        'compileCommands':
                            '${workspaceFolder}/.vscode/toolchains/%s/%s/compile_commands.json' % (env_name, variant),
                        'customConfigurationVariables':
                            properties
                    }
                )

        tasks_file = vscode_node.make_node('tasks.json')
        try:
            tasks = json.loads(json_minify(Utils.readf(tasks_file.abspath(), 'r')))
        except IOError:
            tasks = {'version': '2.0.0', 'tasks': [], 'inputs': []}
        else:
            tasks['tasks'] = [t for t in tasks['tasks'] if not t['label'].startswith('bugengine:')]
            try:
                tasks['inputs'] = [i for i in tasks['inputs'] if not i['id'].startswith('bugengine-')]
            except KeyError:
                tasks['inputs'] = []

        launch_file = vscode_node.make_node('launch.json')
        try:
            launch_config_content = json_minify(Utils.readf(launch_file.abspath(), 'r'))
        except IOError:
            launch_configs = {'version': '0.2.0', 'configurations': [], 'inputs': []}
        else:
            launch_configs = json.loads(launch_config_content)
            launch_configs['configurations'] = [
                c for c in launch_configs['configurations'] if not c['name'].startswith('bugengine:')
            ]
            try:
                launch_configs['inputs'] = [i for i in tasks['inputs'] if not i['id'].startswith('bugengine-')]
            except KeyError:
                launch_configs['inputs'] = []

        for action, command, is_default in [
            ('build', ['build:${input:bugengine-Toolchain}:${input:bugengine-Variant}'], True),
            (
                'build[fail-tests=no]',
                ['build:${input:bugengine-Toolchain}:${input:bugengine-Variant}', '--no-fail-on-tests'], False
            ), ('build[static]', ['build:${input:bugengine-Toolchain}:${input:bugengine-Variant}', '--static'], False),
            ('build[dynamic]', ['build:${input:bugengine-Toolchain}:${input:bugengine-Variant}', '--dynamic'], False),
            ('build[nomaster]', ['build:${input:bugengine-Toolchain}:${input:bugengine-Variant}', '--nomaster'], False),
            ('clean', ['clean:${input:bugengine-Toolchain}:${input:bugengine-Variant}'], False),
            (
                'rebuild', [
                    'clean:${input:bugengine-Toolchain}:${input:bugengine-Variant}',
                    'build:${input:bugengine-Toolchain}:${input:bugengine-Variant}'
                ], False
            ), ('setup', ['setup:${input:bugengine-Toolchain}'], False), ('reconfigure', ['reconfigure'], False),
            (self.cmd, [self.cmd], False)
        ]:
            tasks['tasks'].append(
                {
                    'label': 'bugengine:%s' % action,
                    'type': 'process',
                    'command': [sys.executable],
                    'args': [sys.argv[0]] + command + options,
                    'options': {
                        'cwd': self.srcnode.abspath()
                    },
                    'problemMatcher': ['$gcc', '$msCompile'],
                    'group': {
                        'kind': 'build',
                        'isDefault': True
                    } if is_default else 'build'
                }
            )
            launch_configs['configurations'].append(
                {
                    'name': 'bugengine:waf:%s' % action,
                    'type': 'python',
                    'request': 'launch',
                    'program': sys.argv[0],
                    'args': command + options,
                    'cwd': '${workspaceFolder}',
                }
            )

        tasks['tasks'].append(
            {
                'label': 'bugengine:mypy',
                'type': 'process',
                'command': [sys.executable],
                'args': ['-m', 'mypy', '--show-column-numbers', '-p', 'mypy_root'],
                'options': {
                    'cwd': self.srcnode.abspath()
                },
                'problemMatcher': ['$gcc', '$msCompile'],
                'group': 'build'
            }
        )

        for input in ('Toolchain', 'Variant'):
            tasks['inputs'].append(
                {
                    'id': 'bugengine-%s' % input,
                    'type': 'command',
                    'command': 'cpptools.activeConfigCustomVariable',
                    'args': input
                }
            )

        for g in self.groups:
            for tg in g:
                if not isinstance(tg, TaskGen.task_gen):
                    continue
                if 'bugengine:game' in tg.features:
                    launch_configs['configurations'].append(
                        {
                            'name':
                                'bugengine:%s' % tg.target,
                            'type':
                                'cppdbg',
                            'request':
                                'launch',
                            'program':
                                '${workspaceFolder}/${input:bugengine-Prefix}/${input:bugengine-Variant}/${input:bugengine-Deploy_BinDir}/${input:bugengine-Launcher}',
                            'args': [tg.target],
                            'cwd':
                                '${workspaceFolder}',
                            'miDebuggerPath':
                                '${input:bugengine-DebuggerPath}',
                            'MIMode':
                                '${input:bugengine-DebuggerMode}',
                            'windows': {
                                'type': 'cppvsdbg'
                            },
                            'preLaunchTask':
                                'bugengine:build'
                        }
                    )
                if 'bugengine:python_module' in tg.features:
                    launch_configs['configurations'].append(
                        {
                            'name':
                                'bugengine:%s' % tg.target,
                            'type':
                                'cppdbg',
                            'request':
                                'launch',
                            'program':
                                '${input:bugengine-Python}',
                            'miDebuggerPath':
                                '${input:bugengine-DebuggerPath}',
                            'MIMode':
                                '${input:bugengine-DebuggerMode}',
                            'args': ['-c', 'import py_bugengine; py_bugengine.run()'],
                            'cwd':
                                '${workspaceFolder}/${input:bugengine-Prefix}/${input:bugengine-Variant}/${input:bugengine-Deploy_RunBinDir}',
                            'windows': {
                                'type': 'cppvsdbg'
                            },
                            'preLaunchTask':
                                'bugengine:build'
                        }
                    )
                if 'bugengine:unit_test' in tg.features:
                    unit_test = tg.link_task.outputs[0].path_from(tg.bld.bldnode)
                    launch_configs['configurations'].append(
                        {
                            'name': 'bugengine:%s' % tg.target,
                            'type': 'cppdbg',
                            'request': 'launch',
                            'program': '${input:bugengine-TmpDir}/${input:bugengine-Variant}/%s' % unit_test,
                            'miDebuggerPath': '${input:bugengine-DebuggerPath}',
                            'MIMode': '${input:bugengine-DebuggerMode}',
                            'args': [],
                            'cwd': '${input:bugengine-TmpDir}/${input:bugengine-Variant}',
                            'windows': {
                                'type': 'cppvsdbg'
                            },
                            'preLaunchTask': 'bugengine:build[fail-tests=no]'
                        }
                    )

        for input in (
            'Toolchain', 'Variant', 'Prefix', 'Deploy_RunBinDir', 'Deploy_BinDir', 'Launcher', 'Python', 'DebuggerPath',
            'DebuggerMode', 'TmpDir'
        ):
            launch_configs['inputs'].append(
                {
                    'id': 'bugengine-%s' % input,
                    'type': 'command',
                    'command': 'cpptools.activeConfigCustomVariable',
                    'args': input
                }
            )

        with open(vscode_node.make_node('c_cpp_properties.json').abspath(), 'w') as conf_file:
            json.dump({'configurations': configurations}, conf_file, indent=2)
        with open(tasks_file.abspath(), 'w') as document:
            json.dump(tasks, document, indent=2)
        with open(launch_file.abspath(), 'w') as document:
            json.dump(launch_configs, document, indent=2)


@feature('bugengine:preprocess')
def create_vscode_kernels(task_gen):
    if 'vscode' in task_gen.env.PROJECTS:
        for kernel, kernel_source, kernel_path, kernel_ast in task_gen.kernels:
            kernel_type = 'parse'
            env = task_gen.env
            kernel_env = env
            tgen = task_gen.bld.get_tgen_by_name(env.ENV_PREFIX % task_gen.parent)
            target_suffix = kernel_type
            kernel_target = task_gen.parent + '.' + '.'.join(kernel) + '.' + target_suffix
            kernel_task_gen = task_gen.bld(
                env=kernel_env.derive(),
                bld_env=env,
                target=env.ENV_PREFIX % kernel_target,
                target_name=env.ENV_PREFIX % task_gen.parent,
                safe_target_name=kernel_target.replace('.', '_').replace('-', '_'),
                kernel=kernel,
                features=[
                    'cxx', task_gen.bld.env.STATIC and 'cxxobjects' or 'cxxshlib', 'bugengine:cxx', 'bugengine:kernel',
                    'bugengine:cpu:kernel_create'
                ],
                pchstop=tgen.preprocess.pchstop if tgen.preprocess is not None else None,
                defines=tgen.defines + [
                    'BE_KERNEL_ID=%s_%s' % (task_gen.parent.replace('.', '_'), kernel_target.replace('.', '_')),
                    'BE_KERNEL_NAME=%s' % (kernel_target),
                    'BE_KERNEL_TARGET=%s' % kernel_type
                ],
                variant_name='',
                includes=tgen.includes,
                kernel_source=kernel_ast,
                source=[kernel_source],
                source_nodes=tgen.source_nodes,
                use=tgen.use + [env.ENV_PREFIX % 'plugin.compute.cpu'],
                uselib=tgen.uselib,
            )
            kernel_task_gen.env.PLUGIN = task_gen.env.plugin_name
