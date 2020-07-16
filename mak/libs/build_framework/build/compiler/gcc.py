from waflib import Task, Utils
from waflib.TaskGen import feature, before_method, after_method, extension
from waflib.Tools import msvc
import os


class nm(Task.Task):
    """
    extract all symbols
    """
    color = 'GREEN'

    def run(self):
        def process_objdump_output(lines):
            if self.generator.env.ARCHITECTURE == 'x86':
                # remove cdecl underscore to get symbol name
                remove_leading = 1
            else:
                remove_leading = 0
            seen = set([])
            with open(self.outputs[0].abspath(), 'w') as def_file:
                def_file.write('EXPORTS\n')
                index = 1
                for line in lines:
                    line = line.split('|')
                    if len(line) == 7:
                        name = line[0].strip()
                        status = line[2].strip()
                        if name not in seen:
                            def_file.write('  %s  @%d\n' % (name[remove_leading:], index))
                            index += 1
                            seen.add(name)
            return []

        return self.exec_command(
            [self.generator.env.NM[0], '-g', '--defined-only', '-f', 's'] + [i.abspath() for i in self.inputs],
            filter_stdout=process_objdump_output
        )


@feature('bugengine:export_all')
@after_method('process_source')
@before_method('apply_link')
def generate_def_file_mingw(self):
    if self.env.CC_NAME == 'gcc' and self.env.DEST_BINFMT == 'pe':
        def_file = self.make_bld_node('obj', '', 'exports.def')
        try:
            self.def_files.append(def_file)
        except Exception:
            self.def_files = [def_file]
        tsk = self.create_task('nm', [c.outputs[0] for c in self.compiled_tasks], [def_file])


@feature('cshlib', 'cxxshlib')
@after_method('process_source')
@after_method('generate_def_file')
@after_method('apply_link')
def apply_def_flag_mingw(self):
    self.env.append_value('LINKFLAGS', ['-Wl,-dll'])
    if self.env.CC_NAME == 'gcc' and self.env.DEST_BINFMT == 'pe':
        for f in getattr(self, 'def_files', []):
            self.env.append_value('LINKFLAGS', ['-Wl,-d', f.abspath()])
            self.link_task.dep_nodes.append(f)


def build(bld):
    pass