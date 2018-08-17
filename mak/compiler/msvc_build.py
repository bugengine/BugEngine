from waflib import Task
from waflib.TaskGen import feature, before_method, after_method
from waflib.Tools import msvc
import os


def build(bld):
    cls = Task.classes.get('cpp', None)
    derived = type('cpp', (cls,), {})
    def exec_command_stdout(self, *k, **kw):
        if self.env.CC_NAME == 'msvc':
            with open(self.outputs[0].abspath(), 'w') as out:
                kw['stdout'] = out
                return super(derived, self).exec_command(*k, **kw)
        else:
            return super(derived, self).exec_command(*k, **kw)
    derived.exec_command = exec_command_stdout
    
    def wrap_class(cls_name):
        cls = Task.classes.get(cls_name, None)
        derived = type(cls_name, (cls,), {})
        def exec_command_filter(self, *k, **kw):
            if self.env.CC_NAME == 'msvc':
                kw['filter_stdout'] = lambda x: x[1:]
            if self.env.CC_NAME == 'msvc' and os.path.basename(self.env.LINK_CC[0])[0] in ('I','X'):
                kw['filter_stderr'] = lambda x: x[1:]
            return super(derived, self).exec_command(*k, **kw)
        derived.exec_command = exec_command_filter
    for task in 'c', 'cxx', 'cshlib', 'cxxshlib', 'cstlib', 'cxxstlib', 'cprogram', 'cxxprogram':
        wrap_class(task)


@feature('c', 'cxx')
@after_method('process_source')
def apply_pdb_flag(self):
    if self.env.CC_NAME == 'msvc':
        for task in getattr(self, 'compiled_tasks', []):
            if task:
                task.env.append_unique('CFLAGS', '/Fd%s'%task.outputs[0].change_ext('.pdb').abspath())
                task.env.append_unique('CXXFLAGS', '/Fd%s'%task.outputs[0].change_ext('.pdb').abspath())
