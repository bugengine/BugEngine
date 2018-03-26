from waflib import Task, Context, Utils
from waflib.TaskGen import feature, extension, before_method, after_method
from waflib.Tools import gcc, ccroot
import os


def build(bld):
    bld.platforms.append(bld.external('3rdparty.system.cocoa'))


def plugins(bld):
    pass


@extension('.plist')
def install_plist(self, node):
    self.install_files(os.path.join(self.env.PREFIX, self.bld.optim, self.env.DEPLOY_ROOTDIR),
                       [node])


@feature('cshlib', 'cxxshlib')
@after_method('apply_link')
@after_method('process_use')
def set_osx_shlib_name(self):
    if 'macosx' in self.env.VALID_PLATFORMS:
        if 'plugin' in self.features:
            self.env.append_unique('LINKFLAGS', [
                    '-install_name', os.path.join('@rpath', '..', 'share', 'bugengine', 'plugin', self.link_task.outputs[0].name)
                ])
        elif 'kernel' in self.features:
            self.env.append_unique('LINKFLAGS', [
                    '-install_name', os.path.join('@rpath', '..', 'share', 'bugengine', 'plugin', self.link_task.outputs[0].name)
                ])
        else:
            self.env.append_unique('LINKFLAGS', [
                    '-Wl,-rpath,@loader_path/',
                    '-install_name', os.path.join('@rpath', self.link_task.outputs[0].name)
                ])


@feature('cprogram', 'cxxprogram', 'cshlib', 'cxxshlib')
@after_method('apply_link')
def add_objc_lib(self):
    if 'darwin' in self.env.VALID_PLATFORMS:
        self.env.append_unique('LINKFLAGS', [
                '-lobjc',
            ])


@feature('cprogram', 'cxxprogram')
@after_method('apply_link')
@after_method('process_use')
def set_osx_program_name(self):
    if 'macosx' in self.env.VALID_PLATFORMS:
        self.env.append_unique('LINKFLAGS', [
                '-Wl,-rpath,@loader_path/',
            ])


dsym = '${DSYMUTIL} ${DSYMUTILFLAGS} ${SRC} -o ${TGT[0].parent.parent.abspath()}'
Task.task_factory('dsym', dsym, color='BLUE')
strip = '${STRIP} ${STRIPFLAGS} -S -o ${TGT[0].abspath()} ${SRC[0].abspath()}'
Task.task_factory('strip', strip, color='BLUE')
lipo = '${LIPO} ${LIPOFLAGS} ${SRC} -create -output ${TGT[0].abspath()}'
Task.task_factory('lipo', lipo, color='BLUE')


@feature('multiarch')
@after_method('apply_link')
@after_method('process_use')
def apply_multiarch_darwin(self):
    if 'darwin' in self.env.VALID_PLATFORMS:
        appname = getattr(Context.g_module, Context.APPNAME, self.bld.srcnode.name)

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

        out_rootdir = os.path.join(appname+'.app.dSYM', 'Contents')
        out_rootnode = self.bld.bldnode.make_node(out_rootdir)
        out_dsymdir = out_rootnode.make_node('Resources/DWARF')

        out_node = self.make_bld_node('.bin', None, out_name+'.stripped')
        out_node_dbg = self.make_bld_node('.bin', None, out_name+'.dbg')
        out_node_full = self.make_bld_node('.bin', None, out_name)

        self.lipo_task = self.create_task('lipo', inputs, [out_node_full])
        self.strip_task = self.create_task('strip', [out_node_full], [out_node])
        self.install_as(os.path.join(self.bld.env.PREFIX, self.bld.optim, out_path, out_name),
                        out_node,
                        chmod=Utils.O755)

        dsymtask = getattr(self.bld, 'dsym_task', None)
        if not dsymtask:
            infoplist = self.bld.bldnode.make_node(os.path.join(out_rootdir, 'Info.plist'))
            dsymtask = self.bld.dsym_task = self.create_task('dsym', [], [infoplist])
            self.install_as(os.path.join(self.bld.env.PREFIX, self.bld.optim,
                                         infoplist.path_from(self.bld.bldnode)),
                            infoplist)

        dsymtask.set_inputs(out_node_full)
        dsymtask.set_outputs(out_dsymdir.make_node(out_name))
        self.install_as(os.path.join(self.bld.env.PREFIX, self.bld.optim, appname+'.app.dSYM',
                                     'Contents', 'Resources', 'DWARF', out_name),
                        dsymtask.outputs[-1])

