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


class clangd(Build.BuildContext):
    "creates clangd compile_commands.json"
    cmd = 'clangd'
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

        clangd_node = self.srcnode.make_node('compile_commands.json')

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
                                                    '\t}' % (task.get_cwd().abspath() % vars,
                                                             ", ".join('"%s"' % (c % vars) for c in cmd),
                                                             task.inputs[0].abspath() % vars,
                                                             task.outputs[0].path_from(task.get_cwd()) % vars))
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
