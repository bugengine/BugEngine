from waflib import Task
import sys


class ircc(Task.Task):
    color = 'PINK'

    def run(self):
        return self.exec_command(
            [
                sys.executable, self.generator.env.KERNEL_CLT, '--tmp',
                self.generator.bld.bldnode.parent.parent.abspath(), self.inputs[0].abspath(), self.outputs[0].abspath(),
                self.ircc_target.abspath()
            ]
        )

    def scan(self):
        mak_node = self.generator.bld.bugenginenode.make_node('mak')
        dep_nodes = [mak_node.find_node('tools/bin/ir_compile.py')]
        dep_nodes += mak_node.find_node('libs/ircc').ant_glob('**/*.py')
        dep_nodes += mak_node.find_node('libs/ply').ant_glob('**/*.py')
        dep_nodes += self.ircc_target.ant_glob('**/*.py')
        return (dep_nodes, [])


def build(build_context):
    if not build_context.env.PROJECTS:
        import ircc
        tmp_node = build_context.bldnode.parent.parent
        ircc.ir_parser.IrParser(tmp_node.abspath())
        build_context.env.KERNEL_CLT = build_context.bugenginenode.make_node('mak/tools/bin/ir_compile.py').abspath()