from waflib import Task, Utils
from waflib.TaskGen import feature, before_method, after_method, extension
from waflib.Tools import msvc
import os
import re


class masm(Task.Task):
    """
    run MASM
    """
    color = 'GREEN'
    run_str = '${ML} /nologo /c /Fo ${TGT[0].abspath()} ${SRC}'


class dumpbin(Task.Task):
    """
    extract all symbols
    """
    color = 'GREEN'

    def run(self):
        def process_dumpbin_output(lines):
            symbol = re.compile(
                r'[0-9A-Fa-f]+\s+([0-9A-Fa-f]+)\s+(\w+)\s+\w+\s+(\(\)\s+)?(\w+)\s+\|\s+([a-zA-Z0-9_\$@\?]+)'
            )
            seen = set([])
            with open(self.outputs[0].abspath(), 'w') as def_file:
                def_file.write('EXPORTS\n')
                index = 1
                for line in lines:
                    m = symbol.match(line)
                    if m and int(m[1], 16) != 0 and m[2] != 'UNDEF' and m[4] in ('WeakExternal',
                                                                                 'External') and m[5] not in seen:
                        def_file.write('  %s  @%d\n' % (m[5], index))
                        index += 1
                        seen.add(m[5])
            return []

        return self.exec_command(
            [self.generator.env.DUMPBIN[0], '/SYMBOLS'] + [i.abspath() for i in self.inputs],
            filter_stdout=process_dumpbin_output
        )


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


@feature('bugengine:export_all')
@after_method('process_source')
@before_method('apply_link')
def generate_def_file(self):
    if self.env.CC_NAME == 'msvc':
        def_file = self.make_bld_node('obj', '', 'exports.def')
        try:
            self.def_files.append(def_file)
        except Exception:
            self.def_files = [def_file]
        tsk = self.create_task('dumpbin', [c.outputs[0] for c in self.compiled_tasks], [def_file])


@feature('cshlib', 'cxxshlib')
@after_method('process_source')
@after_method('generate_def_file')
@after_method('apply_link')
def apply_def_flag(self):
    if self.env.CC_NAME == 'msvc':
        for f in getattr(self, 'def_files', []):
            self.env.append_unique('LINKFLAGS', ['/DEF:%s' % f.abspath()])
            self.link_task.dep_nodes.append(f)


def build(bld):
    pass