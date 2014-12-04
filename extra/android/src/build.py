import os
from waflib.TaskGen import feature

def build(bld):
    bld.platforms.append(bld.external('3rdparty.stl-gabi++'))

@feature('multiarch')
def apply_multiarch_android(self):
    if 'android' in self.env.VALID_PLATFORMS:
        for tg_name in self.use:
            task_gen = self.bld.get_tgen_by_name(tg_name)
            task_gen.post()
            if 'cxxshlib' in task_gen.features or 'cxxprogram' in task_gen.features:
                dbg_node = task_gen.link_task.outputs[0]
                out_node = dbg_node.parent.find_or_declare('stripped/%s'%dbg_node.name)
                strip_task = task_gen.create_task('strip', [dbg_node], [out_node])
                outdir = os.path.join(self.bld.env.PREFIX, self.bld.optim, task_gen.env.DEPLOY_BINDIR)
                self.bld.install_files(outdir, out_node)
                self.bld.install_files(outdir, dbg_node, package=False)
