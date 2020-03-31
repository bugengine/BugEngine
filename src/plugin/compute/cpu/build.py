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
            out.write(
                "static const char* s_cpuVariants[] = { %s };\n"
                "static const i32 s_cpuVariantCount = %d;\n"
                "" % (
                    ', '.join('"%s"' % o for o in [''] + [v[1:] for v in self.env.KERNEL_OPTIM_VARIANTS]
                              ), 1 + len(self.env.KERNEL_OPTIM_VARIANTS)
                )
            )


@feature('generate_cpu_variants')
@before_method('process_source')
def generate_cpu_variant_header(self):
    for kernel_name, toolchain in self.env.KERNEL_TOOLCHAINS:
        if kernel_name == 'cpu':
            env = self.bld.all_envs[toolchain]
            out_header = self.make_bld_node('include', None, 'kernel_optims.hh')
            task = self.create_task('cpu_header', [], [out_header])
            task.env = env
            self.env.append_unique('INCLUDES', [out_header.parent.abspath()])


@feature('kernel_create')
def build_cpu_kernels(task_gen):
    task_gen.source = task_gen.kernel_source


def build(build_context):
    build_context.load('kernel_cpu', tooldir=[build_context.path.make_node('mak').abspath()])