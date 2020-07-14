from waflib import Task
from waflib.TaskGen import feature, before_method, after_method, extension
from waflib.Tools import msvc
import os


class masm(Task.Task):
    """
    run MASM
    """
    color = 'GREEN'
    run_str = '${ML} /nologo /c /Fo ${TGT[0].abspath()} ${SRC}'


def wrap_class(cls_name):
    cls = Task.classes.get(cls_name, None)
    derived = type(cls_name, (cls, ), {})

    def exec_command_filter(self, *k, **kw):
        if self.env.CC_NAME == 'msvc':
            kw['filter_stdout'] = lambda x: x[1:]
        if self.env.CC_NAME == 'msvc' and os.path.basename(self.env.LINK_CC[0])[0] in ('I', 'X'):
            kw['filter_stderr'] = lambda x: x[1:]
        return super(derived, self).exec_command(*k, **kw)

    derived.exec_command = exec_command_filter


for task in 'c', 'cxx', 'cshlib', 'cxxshlib', 'cstlib', 'cxxstlib', 'cprogram', 'cxxprogram', 'masm', 'winrc':
    wrap_class(task)


@extension('.masm')
def masm_hook(self, node):
    if self.env.ML:
        return self.create_compiled_task('masm', node)


@feature('c', 'cxx', 'bugengine:kernel')
@after_method('process_source')
@after_method('propagate_uselib_vars')
def apply_pdb_flag(self):
    if self.env.CC_NAME == 'msvc':
        for task in getattr(self, 'compiled_tasks', []) + getattr(self, 'preprocessed_tasks', []):
            if task:
                task.env.append_unique('CFLAGS', '/Fd%s' % task.outputs[0].change_ext('.pdb').abspath())
                task.env.append_unique('CXXFLAGS', '/Fd%s' % task.outputs[0].change_ext('.pdb').abspath())


@feature('cshlib', 'cxxshlib')
@after_method('process_source')
def apply_def_flag(self):
    if self.env.CC_NAME == 'msvc':
        for f in getattr(self, 'def_files', []):
            self.env.append_unique('LINKFLAGS', ['/DEF:%s' % f.abspath()])


def build(bld):
    pass