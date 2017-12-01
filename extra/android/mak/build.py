from waflib.TaskGen import feature, before_method, after_method
from waflib import Context, Node, Utils, Errors, Build, TaskGen
import os


@feature('aapt_resource')
def aapt_resource(self):
    if 'android' in self.env.VALID_PLATFORMS:
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


@feature('dex')
@after_method('apply_java')
@before_method('process_source')
def dex_files(self):
    """
    Create a dex task. There can be only one dex task by task generator.
    """
    if 'android' in self.env.VALID_PLATFORMS:
        destfile = self.destfile
        dexopts = getattr(self, 'dexopts', [])
        basedir = self.basedir
        outdir = self.outdir

        self.dex_task = tsk = self.create_task('dex')
        if not isinstance(destfile, Node.Node):
            destfile = self.path.find_or_declare(destfile)
        if not destfile:
            self.bld.fatal('invalid destfile %r for %r' % (destfile, self))
        tsk.set_outputs(destfile)
        tsk.basedir = basedir
        tsk.outdir = outdir
        tsk.cwd = outdir.abspath()

        if getattr(self, 'javac_task', None):
            tsk.set_run_after(self.javac_task)


def build(bld):
    bld.recurse('tasks.py')
    bld.recurse('install.py')



@feature('multiarch')
def apply_multiarch_android(self):
    pass

@feature('cprogram', 'cxxprogram', 'cshlib', 'cxxshlib')
@before_method('install_step')
@after_method('set_postlink_task')
def strip_debug_info(self):
    if self.env.ENV_PREFIX:
        self.strip_debug_info_impl()

@feature('launcher')
@after_method('install_step')
def install_program_android(self):
    if 'android' in self.env.VALID_PLATFORMS:
        if self.env.ENV_PREFIX: #in multiarch, also install the lib
            self.install_as(os.path.join(self.env.PREFIX, self.bld.optim, self.env.DEPLOY_BINDIR,
                                         self.env.cxxprogram_PATTERN%self.target_name),
                            self.postlink_task.outputs[0],
                            chmod=Utils.O755)

