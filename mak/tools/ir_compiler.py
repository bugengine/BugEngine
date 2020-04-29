from waflib import Task
import sys


class ircc(Task.Task):
    color = 'PINK'

    def run(self):
        return self.exec_command(
            [
                sys.executable, self.generator.env.KERNEL_CLT, '--tmp',
                self.generator.bld.bldnode.abspath(), self.inputs[0].abspath(), self.outputs[0].abspath(),
                self.ircc_target
            ]
        )

    def scan(self):
        return ([], [])


def build(build_context):
    if not build_context.env.PROJECTS:
        import ircc
        ircc.ir_parser.IrParser(build_context.bldnode.abspath())
        build_context.env.TMPDIR = build_context.bldnode.abspath()
        build_context.env.KERNEL_CLT = build_context.bugenginenode.make_node('mak/bin/ir_compile.py').abspath()