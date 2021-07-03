from be_typing import TYPE_CHECKING
from waflib import Build, TaskGen, Task, Options


class unit_test(Task.Task):
    color = 'PINK'

    def sig_vars(self):
        self.m.update(str(Options.options.fail_on_unittest).encode('utf-8'))

    def run(self):
        result = self.exec_command([self.inputs[0].abspath()])
        if Options.options.fail_on_unittest:
            return result
        else:
            return 0


@TaskGen.feature('bugengine:unit_test')
@TaskGen.after_method('apply_link')
@TaskGen.after_method('install_step')
def check_unit_test(task_gen):
    # type: (TaskGen.TaskGen) -> None
    if task_gen.env.UNIT_TESTS and False:
        task = task_gen.create_task('unit_test', [task_gen.postlink_task.outputs[0]])
        task.set_run_after(task_gen.postlink_task)


def build(build_context):
    # type: (Build.BuildContext) -> None
    pass


if TYPE_CHECKING:
    pass