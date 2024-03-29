from waflib.TaskGen import feature, before_method, after_method
from waflib import Options, Context, Node, Utils, Errors, Build, TaskGen
import os


@feature('bugengine:android:aapt_resource')
def aapt_resource(self):
    if 'android' in self.env.VALID_PLATFORMS:
        self.manifest = self.make_bld_node('src', '', 'AndroidManifest.xml')
        self.manifest_task = self.create_task('android_mft', [], self.manifest)
        self.resource_task = tsk = self.create_task('aapt_create', self.resource.ant_glob('**/*'), [self.destfile])
        tsk.env.MANIFEST = self.manifest.abspath()
        tsk.env.RESOURCE_PATH = self.resource.abspath()
        tsk.dep_nodes = [self.manifest]
        nodes = [self.resource]
        while nodes:
            node = nodes.pop()
            if os.path.isdir(node.abspath()):
                nodes.extend([node.make_node(i) for i in node.listdir()])
            else:
                tsk.dep_nodes.append(node)


@feature('javac')
@before_method('apply_java')
def set_dirs(self):
    self.basedir = self.make_bld_node('jar', '', '')
    self.outdir = self.basedir


@feature('dex')
@after_method('apply_java')
@before_method('process_source')
def dex_files(self):
    """
    Create a dex task. There can be only one dex task by task generator.
    """
    if 'android' in self.env.VALID_PLATFORMS:
        dexopts = getattr(self, 'dexopts', [])
        self.outdir.mkdir()
        destfile = self.outdir.find_or_declare(self.destfile)

        self.dex_task = tsk = self.create_task('dex', [], [destfile])
        tsk.basedir = self.basedir
        tsk.outdir = self.outdir
        tsk.cwd = self.outdir.abspath()

        self.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim), [destfile])

        if getattr(self, 'javac_task', None):
            tsk.set_run_after(self.javac_task)


def build(bld):
    if Options.options.android_jdk:
        if not bld.env.env:
            bld.env.env = dict(os.environ)
        bld.env.env['JAVA_HOME'] = Options.options.android_jdk
        bld.env.env['JRE_HOME'] = os.path.join(Options.options.android_jdk, 'jre')

    bld.recurse('tasks.py')
    bld.recurse('install.py')


@feature('bugengine:multiarch')
def apply_multiarch_android(self):
    pass


@feature('cprogram', 'cxxprogram', 'cshlib', 'cxxshlib')
@before_method('install_step')
@after_method('set_postlink_task')
def strip_debug_info(self):
    if self.env.ENV_PREFIX:
        self.strip_debug_info_impl()


@feature('bugengine:launcher')
@after_method('install_step')
def install_program_android(self):
    if 'android' in self.env.VALID_PLATFORMS:
        if self.env.ENV_PREFIX:                                              #in multiarch, also install the lib
            self.install_as(
                os.path.join(
                    self.env.PREFIX, self.bld.optim, self.env.DEPLOY_BINDIR,
                    self.env.cxxprogram_PATTERN % self.target_name
                ),
                self.postlink_task.outputs[0],
                chmod=Utils.O755
            )
