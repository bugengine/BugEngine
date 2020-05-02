from waflib import Context, Build, TaskGen, Options, Utils
import os
import sys


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
        extra_node = self.bugenginenode.make_node('extra')

        folders = []
        commands = []

        for g in self.groups:
            for tg in g:
                if not isinstance(tg, TaskGen.task_gen):
                    continue
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
