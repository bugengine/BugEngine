from waflib.Configure import conf
from waflib import ConfigSet, Context, Build, Configure, Logs, Errors, Options, Utils
import os
try:
    import cPickle as pickle
except ImportError:
    import pickle


OPTION_CONTEXT = None


def autoreconfigure(execute_method):
    """
        Decorator used to set the commands that can be reconfigured automatically
    """
    def execute(self):
        """
            First check if reconfiguration is needed, then triggers the
            normal execute.
        """
        env = ConfigSet.ConfigSet()
        do_config = False
        try:
            env.load(os.path.join(Context.top_dir, Options.lockfile))
        except IOError:
            raise Errors.WafError('The project was not configured: run "waf configure" first!')
        else:
            if env.run_dir != Context.run_dir:
                do_config = True
            else:
                hash_value = 0
                for filename in env['files']:
                    try:
                        hash_value = Utils.h_list((hash_value, Utils.readf(filename, 'rb')))
                    except IOError:
                        do_config = True
                do_config = do_config or (hash_value != env.hash)

        if do_config:
            Logs.warn('wscript files have changed; reconfiguring the project')
            Options.commands.insert(0, self.cmd)
            Options.commands.insert(0, 'reconfigure')
            return "SKIP"

        return execute_method(self)
    return execute


def autosetup(execute_method):
    """
        Decorator used to set the commands that can be setup automatically
    """
    def execute(self):
        """
            First check if setup is needed, then triggers the
            normal execute.
        """
        self.init_dirs()
        do_setup = False
        try:
            env = ConfigSet.ConfigSet()
            env.load(os.path.join(self.cache_dir, Options.lockfile + '.%s' % self.bugengine_variant))
        except (AttributeError, IOError):
            Logs.warn('setup not run; setting up the toolchain')
            do_setup = True
        else:
            hash_value = 0
            for filename in env['files']:
                try:
                    hash_value = Utils.h_list((hash_value, Utils.readf(filename, 'rb')))
                except IOError:
                    do_config = True
            do_setup = (hash_value != env.hash)
            if do_setup:
                Logs.warn('wscript files have changed; setting up the toolchain')

        if do_setup:
            Options.commands.insert(0, self.cmd)
            Options.commands.insert(0, 'setup:%s' % self.bugengine_toolchain)
            return "SKIP"

        return execute_method(self)
    return execute


class ConfigurationContext(Configure.ConfigurationContext):
    """
        ConfigurationContext subclass, which allows to store the current environment used
        for configure so it can be restored during a reconfigure.
    """
    cmd = 'configure'
    def __init__(self, **kw):
        "main init"
        super(ConfigurationContext, self).__init__(**kw)
        self.hash = OPTION_CONTEXT.hash
        self.files = OPTION_CONTEXT.files[:]

    def execute(self):
        """
            Executes the configuration, then stores the current status that can be checked
            during the reconfiguration step
        """
        super(ConfigurationContext, self).execute()
        self.store_options()

    def store_options(self):
        """
            Store last good configuration. This allows the build to automatically
            reconfigure when an environemnt variable changes, or a file changes on disc.
            The file will check common env variable as well as every wscript/python file
            loaded so far.
            Since this is called during the configuration step, the wscript/python files
            loaded during the build step only will not cause a reconfigure, which is the
            desired behaviour.
        """
        Options.options.environ = dict(os.environ)
        node = self.bldnode.make_node('options.pickle')
        pickle.dump(Options.options, open(node.abspath(), 'wb'), protocol=2)

    def store(self):
        """
            Cleans up the cache before storing more in there
        """
        self.cachedir = self.bldnode.find_node(Build.CACHE_DIR)
        if self.cachedir:
            lst = self.cachedir.ant_glob('**', quiet=True)
            for x in lst:
                x.delete()

        super(ConfigurationContext, self).store()


class ReconfigurationContext(ConfigurationContext):
    "reconfigures the project with the same options as the last call to configure"
    cmd = 'reconfigure'
    def __init__(self, **kw):
        "main init"
        super(ReconfigurationContext, self).__init__(**kw)

    def execute(self):
        """
            restores the environment as it was during the last run, then reconfigures
            the project.
        """
        opt = Options.options
        self.restore_options()
        super(ReconfigurationContext, self).execute()
        Options.options = opt

    def restore_options(self):
        """
            Restores the environment as it was during the configure step. This allows
            reconfigure to discover the same compilers, even if the reconfigure step
            is not started in the same environment as configure (e.g. called from an IDE)
        """
        self.init_dirs()
        try:
            node = self.bldnode.make_node('options.pickle')
            Options.options = pickle.load(open(node.abspath(), 'rb'))
            os.environ.clear()
            os.environ.update(Options.options.environ)
            self.environ = dict(os.environ)
        except IOError:
            raise Errors.WafError('Option file not found; configure the project first')


class Setup(Configure.ConfigurationContext):
    cmd = '_setup'

    def __init__(self, **kw):
        super(Setup, self).__init__(**kw)
        self.hash = OPTION_CONTEXT.hash
        self.files = OPTION_CONTEXT.files[:]

    def load_envs(self):
        """
        The configuration command creates files of the form ``build/c4che/NAMEcache.py``. This method
        creates a :py:class:`waflib.ConfigSet.ConfigSet` instance for each ``NAME`` by reading those
        files and stores them in :py:attr:`waflib.Build.BuildContext.allenvs`.
        """
        self.cachedir = self.bldnode.find_node(Build.CACHE_DIR)
        if not self.cachedir:
            raise Errors.WafError('The project was not configured: run "waf configure" first!')
        lst = self.cachedir.ant_glob('**/*%s' % Build.CACHE_SUFFIX, quiet=True)

        if not lst:
            raise Errors.WafError('The cache directory is empty: reconfigure the project')

        for x in lst:
            name = x.path_from(self.cachedir).replace(Build.CACHE_SUFFIX, '').replace('\\', '/')
            env = ConfigSet.ConfigSet(x.abspath())
            self.all_envs[name] = env
            for f in env[Build.CFG_FILES]:
                newnode = self.root.find_resource(f)
                if not newnode or not newnode.exists():
                    raise Errors.WafError('Missing configuration file %r, reconfigure the project!' % f)

    def setup(self, tool, tooldir=None, funs=None):
        if isinstance(tool, list):
            for i in tool:
                self.setup(i, tooldir)
            return

        module = Context.load_tool(tool, tooldir)
        if hasattr(module, "setup"):
            module.setup(self)
        self.tools.append({'tool': tool, 'tooldir': tooldir, 'funs': funs})

    @autoreconfigure
    def execute(self):
        self.init_dirs()

        self.load_envs()
        try:
            env = ConfigSet.ConfigSet(os.path.join(self.cachedir.abspath(), 'build.config.py'))
        except EnvironmentError:
            pass
        else:
            if env.version < Context.HEXVERSION:
                raise Errors.WafError('Project was configured with a different version of Waf, please reconfigure it')

            for t in env.tools:
                self.setup(**t)

        path = os.path.join(self.bldnode.abspath(), 'setup-%s.log' % self.bugengine_variant)
        self.logger = Logs.make_logger(path, 'cfg')

        self.env = self.all_envs[self.bugengine_variant]
        self.recurse([Context.run_dir])
        self.store()
        env = ConfigSet.ConfigSet()
        env.files = self.files
        env.hash = self.hash
        env.store(os.path.join(self.bldnode.find_node(Build.CACHE_DIR).abspath(), Options.lockfile + '.%s.setup' % self.bugengine_variant))


Build.BuildContext.execute = autoreconfigure(autosetup(Build.BuildContext.execute))


def add_setup_command(toolchain):
    class Command(Setup):
        cmd = 'setup:%s' % (toolchain)
        bugengine_variant = toolchain
        bugengine_toolchain = toolchain
        variant = toolchain
        fun = 'setup'


def add_build_command(toolchain, optimisation):
    c = {}
    for command in (Build.BuildContext, Build.CleanContext, Build.ListContext):
        name = command.__name__.replace('Context', '').lower()

        class Command(command):
            optim = optimisation
            cmd = name + ':' + toolchain + ':' + optimisation
            bugengine_toolchain = toolchain
            bugengine_variant = toolchain + '.setup'
            variant = os.path.join(toolchain, optimisation)

        c[command] = Command


    class Deploy(c[Build.BuildContext]):
        cmd = 'deploy:%s:%s' % (toolchain, optimisation)
        bugengine_variant = toolchain + '.setup'
        bugengine_toolchain = toolchain
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


def add_all_build_commands(env):
    for toolchain in env.ALL_TOOLCHAINS:
        add_setup_command(toolchain)
        for optim in env.ALL_VARIANTS:
            add_build_command(toolchain, optim)



@conf
def rebuild_context_commands(configuration_context):
    add_all_build_commands(configuration_context.env)


for command in ['build', 'clean']:
    for variant in ['debug', 'profile', 'final']:

        class BuildWrapperVariant(Build.BuildContext):
            cmd = '%s:all:%s' % (command, variant)

            def execute(self):
                self.restore()
                if not self.all_envs:
                    self.load_envs()
                for toolchain in self.env.ALL_TOOLCHAINS:
                    cmd, all, variant = self.cmd.split(':')
                    Options.commands.append('%s:%s:%s' % (cmd, toolchain, variant))

    class BuildWrapperAll(Build.BuildContext):
        cmd = '%s:all' % command

        def execute(self):
            self.restore()
            if not self.all_envs:
                self.load_envs()
            for toolchain in self.env.ALL_TOOLCHAINS:
                for variant in self.env.ALL_VARIANTS:
                    Options.commands.append('%s:%s:%s' % (self.cmd[:-4], toolchain, variant))


def options(option_context):
    #for command in (Build.BuildContext, Build.CleanContext, Build.ListContext):
    #    command.cmd = '_%s'%command.cmd
    global OPTION_CONTEXT
    OPTION_CONTEXT = option_context
    try:
        env = ConfigSet.ConfigSet('.waf_toolchains.py')
        add_all_build_commands(env)
    except Exception as e:
        pass