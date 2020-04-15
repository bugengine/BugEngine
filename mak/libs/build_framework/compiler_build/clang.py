import tempfile
from waflib import Task


def clang_exec_command(exec_command):
    def exec_command_response_file(task, cmd, **kw_args):
        command = []
        resp_file_arguments = []
        inputs = set((x.bldpath() for x in task.inputs))
        for arg in cmd[1:]:
            if arg in inputs:
                resp_file_arguments.append(arg)
            elif arg[0:2] in ('-I', '-L', '-D'):
                resp_file_arguments.append(arg)
            else:
                command.append(arg)
        with tempfile.NamedTemporaryFile('w+') as response_file:
            response_file.write('\n'.join(resp_file_arguments))
            response_file.flush()
            return exec_command(task, [cmd[0], '@%s'%response_file.name] + command, **kw_args)
    return exec_command_response_file


def build(bld):
    if bld.env.COMPILER_NAME == 'clang':
        for cls_name in 'c', 'cxx', 'cshlib', 'cxxshlib', 'cprogram', 'cxxprogram':
            cls = Task.classes.get(cls_name, None)
            derived = type(cls_name, (cls, ), {})
            derived.exec_command = clang_exec_command(derived.exec_command)
