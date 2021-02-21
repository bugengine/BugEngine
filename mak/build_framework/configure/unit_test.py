from be_typing import TYPE_CHECKING
from waflib import Configure, TaskGen, Errors, Task


class run_test_exe(Task.Task):
    color = 'PINK'

    def run(self):
        return self.exec_command([self.inputs[0].abspath()])


@TaskGen.feature('check_unit_test')
@TaskGen.after_method('apply_link')
def check_run_exe(task_gen):
    # type: (TaskGen.TaskGen) -> None
    task = task_gen.create_task('run_test_exe', [task_gen.link_task.outputs[0]])
    task.set_run_after(task_gen.link_task)


def configure(configuration_context):
    # type: (Configure.ConfigurationContext) -> None
    try:
        configuration_context.check(
            msg='Checking if unit tests can be run on host',
            features=['check_unit_test', 'c', 'cprogram'],
            fragment='int main() { return 0; }'
        )
    except Errors.WafError:
        pass
    else:
        configuration_context.env.UNIT_TESTS = True


if TYPE_CHECKING:
    pass