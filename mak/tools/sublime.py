from waflib import Context, Build, TaskGen, Options, Utils
import os
import sys
import re


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
            except Exception:
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


class sublime3(Build.BuildContext):
    "creates workspace for Sublime Text"
    cmd = 'sublime'
    fun = 'build'
    optim = 'debug'
    variant = '%(toolchain)s/%(optim)s'
    #bugengine_variant = '%(bugengine_variant)s'

    def execute(self):
        """
        Entry point
        """
        Options.options.nomaster = True
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

        workspace_node = self.srcnode.make_node('%s.sublime-project' % appname)
        clangd_node = self.srcnode.make_node('compile_commands.json')
        extra_node = self.bugenginenode.make_node('extra')

        folders = []
        commands = []

        for g in self.groups:
            for tg in g:
                if not isinstance(tg, TaskGen.task_gen):
                    continue
                if 'kernel' not in tg.features:
                    tg.post()
                    for task in tg.tasks:
                        if task.__class__.__name__ in ('cxx', 'c', 'objc', 'objcxx'):
                            for env_name in self.env.ALL_TOOLCHAINS:
                                env = self.all_envs[env_name]
                                if env.SUB_TOOLCHAINS:
                                    bld_env = self.all_envs[env.SUB_TOOLCHAINS[0]].derive()
                                else:
                                    bld_env = env.derive()
                                includes, defines = gather_includes_defines(tg)
                                bld_env.append_value('INCPATHS', includes + bld_env.INCLUDES + bld_env.SYSTEM_INCLUDES)
                                bld_env.append_value('DEFINES', defines)
                                cmd = self.expand_cmd(task, bld_env)
                                for variant in self.env.ALL_VARIANTS:
                                    vars = {'toolchain': env_name, 'optim': variant}
                                    commands.append('\t{\n'
                                                    '\t\t"directory": "%s",\n'
                                                    '\t\t"arguments": [%s],\n'
                                                    '\t\t"file": "%s",\n'
                                                    '\t\t"output": "%s"\n'
                                                    '\t}' % (task.get_cwd().path_from(self.srcnode) % vars,
                                                             ", ".join('"%s"' % (c % vars) for c in cmd),
                                                             task.inputs[0].abspath() % vars,
                                                             task.outputs[0].path_from(task.get_cwd()) % vars))
                for node in tg.source_nodes:
                    name = tg.name
                    path = node.path_from(self.srcnode).replace('\\', '/')
                    if node.is_child_of(extra_node):
                        while node.parent != extra_node:
                            node = node.parent
                        name += '[%s]' % node.name
                    folders.append('\t\t{\n\t\t\t"name": "%s",\n\t\t\t"path": "%s"\n\t\t}' % (name, path))

        systems = []
        for env_name in self.env.ALL_TOOLCHAINS:
            for variant in self.env.ALL_VARIANTS:
                systems.append('\t\t{\n'
                               '\t\t\t"name": "%s - %s",\n'
                               '\t\t\t"cmd": ["%s", "%s", "build:%s:%s"]\n'
                               '\t\t}' % (env_name, variant, sys.executable, sys.argv[0], env_name, variant))

        with open(workspace_node.abspath(), 'w') as workspace:
            workspace.write('{\n\t"folders":\n\t[\n')
            workspace.write(',\n'.join(folders))
            workspace.write('\n'
                            '\t],\n'
                            '\t"settings":\n'
                            '\t{\n'
                            '\t\t"LSP":\n'
                            '\t\t{\n'
                            '\t\t\t"pyls":\n'
                            '\t\t\t{\n'
                            '\t\t\t\t"enabled": true\n'
                            '\t\t\t},\n'
                            '\t\t\t"clangd":\n'
                            '\t\t\t{\n'
                            '\t\t\t\t"enabled": true\n'
                            '\t\t\t}\n'
                            '\t\t}\n'
                            '\t},\n'
                            '\t"build_systems":\n'
                            '\t[\n'
                            '%s'
                            '\t]\n'
                            '}\n' % (',\n'.join(systems)))

        with open(clangd_node.abspath(), 'w') as clangd:
            clangd.write('[\n')
            clangd.write(',\n'.join(commands))
            clangd.write('\n]')


    pattern = re.compile('\${([^}]+)}')
    def expand_cmd(self, task, env):
        def get_var(name):
            if name == 'SRC':
                return 'task.inputs'
            elif name == 'TGT':
                return 'task.outputs'
            else:
                return 'Utils.to_list(env.%s)'%name
        def expand_var(match):
            task # publish task as a local variable in locals()
            expression = match.group(1)
            expansion = expression.find(':')
            if expansion != -1:
                radix = env[expression[:expansion]]
                expression = expression[expansion+1:]
            else:
                radix = '%s'
            execution = expression.find('.')
            if execution != -1:
                var = expression[:execution]
                code = expression[execution:]
            else:
                var = expression
                code = ''
            subvalue = var.find('[')
            if subvalue != -1:
                index = var[subvalue:]
                var = get_var(var[:subvalue])
                var = var+index
            else:
                var = get_var(var)
            var = eval(var+code, locals(), globals())
            if var:
                return ' '.join(radix%x for x in Utils.to_list(var))
            else:
                return None
        cmd = self.pattern.sub(expand_var, task.orig_run_str).split()
        return cmd