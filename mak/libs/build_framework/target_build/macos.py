from waflib import Task, Context, Utils
from waflib.TaskGen import taskgen_method, feature, extension, before_method, after_method
from waflib.Tools import gcc, ccroot
import os


def build(bld):
    bld.platforms.append(bld.external('3rdparty.system.cocoa'))
    def wrap_class(cls_name):
        cls = Task.classes.get(cls_name, None)
        derived = type(cls_name, (cls, ), {})
        def exec_command_clean(self, *k, **kw):
            self.outputs[0].delete(evict=False)
            return cls.exec_command(self, *k, **kw)
        derived.exec_command = exec_command_clean
    for cls_name in 'cshlib', 'cxxshlib', 'cprogram', 'cxxprogram', 'lipo':
        wrap_class(cls_name)


def plugins(bld):
    pass


@extension('.plist')
def install_plist_darwin(self, node):
    if 'darwin' in self.bld.env.VALID_PLATFORMS:
        bld_env = self.bld.env
        if bld_env.SUB_TOOLCHAINS:
            bld_env = self.bld.all_envs[bld_env.SUB_TOOLCHAINS[0]]
        self.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, bld_env.DEPLOY_ROOTDIR), [node])


@feature('cshlib', 'cxxshlib')
@after_method('apply_link')
@after_method('process_use')
def set_osx_shlib_name(self):
    if 'macosx' in self.env.VALID_PLATFORMS:
        if 'plugin' in self.features:
            self.env.append_unique('LINKFLAGS', [
                '-install_name',
                os.path.join('@executable_path', '..', 'share', 'bugengine', 'plugin', self.link_task.outputs[0].name)
            ])
        elif 'kernel' in self.features:
            self.env.append_unique('LINKFLAGS', [
                '-install_name',
                os.path.join('@executable_path', '..', 'share', 'bugengine', 'plugin', self.link_task.outputs[0].name)
            ])
        else:
            self.env.append_unique(
                'LINKFLAGS',
                ['-install_name', os.path.join('@loader_path', self.link_task.outputs[0].name)])


@feature('cprogram', 'cxxprogram', 'cshlib', 'cxxshlib')
@after_method('apply_link')
def add_objc_lib(self):
    if 'darwin' in self.env.VALID_PLATFORMS:
        self.env.append_unique('LINKFLAGS', [
            '-l', 'objc',
        ])


@feature('cprogram', 'cxxprogram')
@after_method('apply_link')
@after_method('process_use')
def set_osx_program_name(self):
    pass


dsym = '${DSYMUTIL} ${DSYMUTILFLAGS} ${SRC} -o ${TGT[0].parent.parent.abspath()}'
Task.task_factory('dsym', dsym, color='BLUE')
strip = '${STRIP} ${STRIPFLAGS} -S -x -o ${TGT[0].abspath()} ${SRC[0].abspath()}'
Task.task_factory('strip', strip, color='BLUE')
lipo = '${LIPO} ${LIPOFLAGS} ${SRC} -create -output ${TGT[0].abspath()}'
Task.task_factory('lipo', lipo, color='BLUE')


class codesign(Task.Task):
    def run(self):
        with open(self.outputs[0].abspath(), 'wb') as out:
            out.write(self.inputs[0].read('rb'))
        return self.exec_command(self.env.CODESIGN + ['-s-', self.outputs[0].abspath()])


@taskgen_method
def darwin_postlink_task(self, link_task):
    appname = getattr(Context.g_module, Context.APPNAME, self.bld.srcnode.name)

    bldnode = self.bld.bldnode
    out_rootdir = os.path.join(appname + '.app.dSYM', 'Contents')
    out_rootnode = bldnode.make_node(out_rootdir)
    out_dsymdir = out_rootnode.make_node('Resources/DWARF')

    out_node = link_task.outputs[0]
    out_node_stripped = out_node.change_ext('.stripped')
    out_node_signed = self.make_bld_node('bin.signed', None, out_node.name)
    self.strip_task = self.create_task('strip', [out_node], [out_node_stripped])
    self.sign_task = self.create_task('codesign', [out_node_stripped], [out_node_signed])
    self.link_task = self.sign_task

    dsymtask = getattr(self.bld, 'dsym_task', None)
    if not dsymtask:
        infoplist = out_rootnode.make_node('Info.plist')
        dsymtask = self.bld.dsym_task = self.create_task('dsym', [], [infoplist])
        self.install_as(os.path.join(self.bld.env.PREFIX, self.bld.optim, infoplist.path_from(bldnode)), infoplist)

    dsymtask.set_inputs(out_node)
    dsymtask.set_outputs(out_dsymdir.make_node(out_node.name))
    self.install_as(
        os.path.join(self.bld.env.PREFIX, self.bld.optim, appname + '.app.dSYM', 'Contents', 'Resources', 'DWARF',
                     out_node.name), dsymtask.outputs[-1])
    return self.sign_task


@feature('cshlib', 'cxxshlib', 'cprogram', 'cxxprogram')
@after_method('apply_link')
@after_method('process_use')
@before_method('install_step')
def apply_postlink_darwin(self):
    if 'darwin' in self.env.VALID_PLATFORMS:
        if not self.env.ENV_PREFIX:
            self.darwin_postlink_task(self.link_task)


@feature('multiarch')
@after_method('apply_link')
@after_method('process_use')
@after_method('apply_postlink_darwin')
@before_method('install_step')
def apply_multiarch_darwin(self):
    if 'darwin' in self.env.VALID_PLATFORMS:
        features = []
        inputs = []
        for tg_name in self.use:
            task_gen = self.bld.get_tgen_by_name(tg_name)
            task_gen.post()
            if getattr(task_gen, 'link_task', None):
                inputs.append(task_gen.link_task.outputs[0])
                features += task_gen.features
        if 'plugin' in features:
            out_name = task_gen.env.cxxshlib_PATTERN % self.target
            out_path = self.env.DEPLOY_PLUGINDIR
        elif 'kernel' in features:
            out_name = task_gen.env.cxxshlib_PATTERN % self.target
            out_path = self.env.DEPLOY_KERNELDIR
        elif 'cshlib' in features or 'cxxshlib' in features:
            out_name = task_gen.env.cxxshlib_PATTERN % self.target
            out_path = self.env.DEPLOY_RUNBINDIR
        elif 'cprogram' in features or 'cxxprogram' in features:
            out_name = task_gen.env.cxxprogram_PATTERN % self.target
            out_path = self.env.DEPLOY_BINDIR
        else:
            return

        out_node_full = self.make_bld_node('bin', None, out_name)

        self.lipo_task = self.create_task('lipo', inputs, [out_node_full])
        l = self.darwin_postlink_task(self.lipo_task)
        self.install_as(os.path.join(self.bld.env.PREFIX, self.bld.optim, out_path, out_name),
                        l.outputs[0],
                        chmod=Utils.O755)


def deploy(build_context):
    pass


def run(build_context):
    pass


def debug(build_context):
    pass
