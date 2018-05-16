from waflib import Task, Errors
from waflib.TaskGen import feature, extension, before_method
import os
import sys


class cpu_header(Task.Task):
    color = 'PINK'

    def scan(self):
        return ([], [])

    vars = ['KERNEL_OPTIM_VARIANTS']

    def run(self):
        with open(self.outputs[0].abspath(), 'w') as out:
            out.write("static const char* s_cpuVariants[] = { %s };\n"
                      "static const i32 s_cpuVariantCount = %d;\n"
                      "" % (', '.join('"%s"'%o for o in [''] + [v[1:] for v in self.env.KERNEL_OPTIM_VARIANTS]),
                            1 + len(self.env.KERNEL_OPTIM_VARIANTS)))


@feature('generate_cpu_variants')
@before_method('process_source')
def generate_cpu_variants(self):
    for kernel_name, toolchain in self.env.KERNEL_TOOLCHAINS:
        if kernel_name == 'cpu':
            env = self.bld.all_envs[toolchain]
            out_header = self.make_bld_node('include', None, 'kernel_optims.hh')
            task = self.create_task('cpu_header', [], [out_header])
            task.env = env
            self.env.append_unique('INCLUDES', [out_header.parent.abspath()])


class clt(Task.Task):
    color   = 'PINK'
    run_str = """%s
                 ${KERNEL_CLT}
                 --tmp ${TMPDIR}
                 ${SRC[0].abspath()}
                 ${TGT[0].path_from(bld.bldnode)}
                 ${env.KERNEL_CLT_TARGET.abspath()}""" % sys.executable.replace('\\', '/')
    def scan(self):
        return ([], [])


@feature('cpu')
@extension('.ii')
def kernel_build_cpu_source(self, source):
    target_env = self.kernel_env
    preprocessed = self.make_bld_node('src', source.parent.make_node('cpu'), source.name[:-2]+'cc')
    t = self.create_task('clt', [source], [preprocessed])
    t.path = self.bld.variant_dir
    t.env.KERNEL_CLT = self.bld.bugenginenode.find_node('mak/tools/clt.py').abspath()
    t.env.TMPDIR = self.bld.bldnode.abspath()
    t.dep_nodes = [self.bld.bugenginenode.find_node('mak/tools/clt.py')]
    t.dep_nodes += self.bld.bugenginenode.find_node('mak/libs/clt').ant_glob('**/*.py')
    t.dep_nodes += self.bld.bugenginenode.find_node('mak/libs/ply').ant_glob('**/*.py')
    t.dep_nodes += self.bld.env.KERNEL_CLT_TARGET.ant_glob('**/*.py')
    self.source.append(preprocessed)


def build(build_context):
    for kernel_name, toolchain in build_context.env.KERNEL_TOOLCHAINS:
        if kernel_name == 'cpu':
            env = build_context.all_envs[toolchain]
            env.KERNEL_CLT_TARGET = build_context.path.find_node('mak/clc_cwriter')
