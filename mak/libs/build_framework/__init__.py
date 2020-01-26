"""
    This is the main entry point for cofiguration and build.
    This module is responsible for loading the tools, discovering the host
    and the targets and launching their respective configuration and builds,
    as well as the reconfigure target
"""

import os
import sys
from waflib import Configure, Build, ConfigSet, Context, Utils, Options, Logs, Errors, TaskGen
try:
    import cPickle as pickle
except ImportError:
    import pickle
from . import device
from . import build_step
from . import host
from . import compiler_conf
from . import target_conf
from . import checks
from . import install

Utils.run_process = Utils.run_regular_process
Utils.get_process = Utils.alloc_process_pool = Utils.nada


def options(opt):
    # type: (Options.OptionsContext) -> None
    "Creates main option groups and load options for the host and all targets"
    opt.add_option_group('SDK paths and options')
    opt.add_option_group('3rd party libraries')
    opt.bugenginenode = opt.path

    opt.load('visualstudio', tooldir=[os.path.join(opt.path.abspath(), 'mak', 'tools')])
    opt.load('xcode', tooldir=[os.path.join(opt.path.abspath(), 'mak', 'tools')])
    opt.load('netbeans', tooldir=[os.path.join(opt.path.abspath(), 'mak', 'tools')])
    opt.load('eclipse', tooldir=[os.path.join(opt.path.abspath(), 'mak', 'tools')])
    opt.load('qtcreator', tooldir=[os.path.join(opt.path.abspath(), 'mak', 'tools')])
    opt.load('vscode', tooldir=[os.path.join(opt.path.abspath(), 'mak', 'tools')])
    opt.load('sublime', tooldir=[os.path.join(opt.path.abspath(), 'mak', 'tools')])

    device.options(opt)
    build_step.options(opt)
    host.options(opt)
    compiler_conf.options(opt)
    target_conf.options(opt)

    third_party_node = opt.bugenginenode.make_node('src').make_node('3rdparty')
    for category in third_party_node.listdir():
        category_node = third_party_node.make_node(category)
        for third_party in category_node.listdir():
            opt.recurse('src/3rdparty/%s/%s/%s.py'%(category, third_party, third_party))
    option_file = opt.bugenginenode.find_node('options')
    if option_file:
        with open(option_file.abspath(), 'r') as file:
            opt.parse_args(file.readlines())


def configure(conf):
    # type: (Configure.ConfigurationContext) -> None
    "Recursively calls configure on host and all targets to create all available toolchains"
    conf.bugenginenode = conf.path
    checks.configure(conf)
    host.configure(conf)
    extra = conf.bugenginenode.make_node('extra')
    for extra_platform in extra.listdir():
        directory = extra.make_node(extra_platform).abspath()
        if os.path.isdir(directory) and os.path.isfile(os.path.join(directory, 'wscript')):
            conf.recurse(extra.make_node(extra_platform).abspath(), name='host_configure')
    conf.load('flex', tooldir=[os.path.join(conf.path.abspath(), 'mak', 'tools')])
    conf.load('bison', tooldir=[os.path.join(conf.path.abspath(), 'mak', 'tools')])
    compiler_conf.configure(conf)
    target_conf.configure(conf)
    conf.env.ALL_TOOLCHAINS.sort(key=lambda x: x.split('-'))


def setup(conf):
    # type: (Configure.ConfigurationContext) -> None
    "setup step before the build: recursively cals setup on every third party library"
    third_party_node = conf.bugenginenode.make_node('src').make_node('3rdparty')
    extra = conf.bugenginenode.make_node('extra')
    for category in third_party_node.listdir():
        conf.start_msg('      `- [%s]' % category)
        category_node = third_party_node.make_node(category)
        for third_party in category_node.listdir():
            conf.recurse('src/3rdparty/%s/%s/%s.py'%(category, third_party, third_party),
                         name='setup',
                         once=False)
        conf.end_msg(' ')
    if conf.env.VALID_PLATFORMS:
        extra_dir = os.path.join(conf.bugenginenode.abspath(), 'extra', conf.env.VALID_PLATFORMS[0])
        if os.path.isdir(extra_dir):
            conf.recurse(extra_dir, name='setup', once=False)


def build(bld):
    # type: (Build.BuildContext) -> None
    "Loads main build file as well as the target-specific build file that can declare extra modules"
    from . import compiler_build
    from . import target_build
    bld.bugenginenode = bld.path
    bld.common_env = bld.env
    if not bld.env.PROJECTS:
        if bld.bugengine_variant == '':
            raise Errors.WafError('Call %s %s %s:toolchain:variant, '
                                  'with toolchain in\n\t%s\nvariant in\n\t%s' %
                                  (sys.executable, sys.argv[0],
                                   bld.cmd,
                                   '\n\t'.join(bld.env.ALL_TOOLCHAINS),
                                   '\n\t'.join(bld.env.ALL_VARIANTS)))
        bld.env = bld.all_envs[bld.bugengine_variant]
    bld.platforms = []
    build_step.build(bld)
    host.build(bld)
    target_build.build(bld)
    compiler_build.build(bld)
    install.build(bld)

    if bld.env.PROJECTS:
        def rc_hook(self, node):
            # type: (TaskGen.task_gen, Node.Node) -> None
            "creates RC hook to silence waf error"
            # pylint: disable=unused-argument
            pass
        if '.rc' not in TaskGen.task_gen.mappings:
            TaskGen.task_gen.mappings['.rc'] = rc_hook


def deploy(build_context):
    # type: (Build.BuildContext) -> None
    from . import target_build
    target_build.deploy(build_context)


def plugins(build_context):
    # type: (Build.BuildContext) -> None
    """
        Recursively calls plugins for host/target so each OS can declare their own platform-specific
        plugins
    """
    from . import target_build
    host.plugins(build_context)
    target_build.plugins(build_context)


class ConfigurationContext(Configure.ConfigurationContext):
    """
        ConfigurationContext subclass, which allows to store the current environment used
        for configure so it can be restored during a reconfigure.
    """
    cmd = 'configure'

    def store(self):
        # type: () -> None
        """
            Stores the current status that can be checked during the reconfiguration step
        """
        super(ConfigurationContext, self).store()
        self.store_options()
        for m in sys.modules.values():
            try:
                if m.__file__.startswith(self.path.abspath()):
                    if m.__file__ not in self.files:
                        self.files.append(m.__file__)
                        self.hash = Utils.h_list((self.hash, Utils.readf(m.__file__, 'rb')))
            except (AttributeError, OSError):
                pass

    def store_options(self):
        # type: () -> None
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


class ReconfigurationContext(Configure.ConfigurationContext):
    "reconfigures the project with the same options as the last call to configure"
    cmd = 'reconfigure'

    def execute(self):
        # type: () -> None
        """
            restores the environment as it was during the last run, then reconfigures
            the project.
        """
        opt = Options.options
        self.restore_options()
        super(ReconfigurationContext, self).execute()
        Options.options = opt

    def restore_options(self):
        # type: () -> None
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


def autoreconfigure(execute_method):
    # type: (Any) -> Any
    """
        Decorator used to set the commands that can be reconfigured automatically
    """

    def execute(self):
        # type: (Build.BuildContext) -> str
        """
            First check if reconfiguration is needed, then triggers the
            normal execute.
        """
        env = ConfigSet.ConfigSet()
        do_config = False
        env.load(os.path.join(Context.top_dir, Options.lockfile))
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


setattr(Build.BuildContext, 'execute', autoreconfigure(Build.BuildContext.execute))


class ListToolchainsContext(Build.BuildContext):
    """
        Command that will print all available toolchains to stdout
    """
    cmd = 'list_toolchains'

    def execute(self):
        # type: () -> None
        self.restore()
        if not self.all_envs:
            self.load_envs()
        for toolchain in self.env.ALL_TOOLCHAINS:
            print(toolchain)


class ListVariantsContext(Build.BuildContext):
    """
        Command that will print all available variants to stdout
    """
    cmd = 'list_variants'

    def execute(self):
        # type: () -> None
        self.restore()
        if not self.all_envs:
            self.load_envs()
        for variant in self.env.ALL_VARIANTS:
            print(variant)


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from waflib import Node
    from typing import Any
