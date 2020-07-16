from waflib import Task, Utils, Errors
from waflib.TaskGen import feature, before_method, after_method, extension
from waflib.Tools import msvc
import os
import re


class dumpbin(Task.Task):
    """
    extract all symbols
    """
    color = 'GREEN'

    def run(self):
        def process_dumpbin_output(lines):
            symbol = re.compile(
                r'[0-9A-Fa-f]+\s+([0-9A-Fa-f]+)\s+(\w+)\s+\w+\s+(\(\)\s+)?(\w+)\s+\|\s+([a-zA-Z0-9_\$@\?<>\-]+)'
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
def generate_def_file(self):
    if self.env.DEST_BINFMT == 'pe':
        def_file = self.make_bld_node('obj', '', 'exports.def')
        try:
            self.def_files.append(def_file)
        except Exception:
            self.def_files = [def_file]
        if self.env.DUMPBIN:
            self.create_task('dumpbin', [c.outputs[0] for c in self.compiled_tasks], [def_file])
        elif self.env.NM:
            self.create_task('nm', [c.outputs[0] for c in self.compiled_tasks], [def_file])
        else:
            raise Errors.WafError('no tool available to extract symbol list')


@feature('cshlib', 'cxxshlib')
@after_method('process_source')
@after_method('generate_def_file')
@after_method('apply_link')
def apply_def_flag(self):
    if self.env.CC_NAME == 'gcc' and self.env.DEST_BINFMT == 'pe':
        self.env.append_value('LINKFLAGS', ['-Wl,-dll'])
        for f in getattr(self, 'def_files', []):
            self.env.append_value('LINKFLAGS', ['-Wl,-d', f.abspath()])
            self.link_task.dep_nodes.append(f)
    if self.env.CC_NAME == 'msvc':
        for f in getattr(self, 'def_files', []):
            self.env.append_unique('LINKFLAGS', ['/DEF:%s' % f.abspath()])
            self.link_task.dep_nodes.append(f)


def build(bld):
    bld.platforms.append(bld.external('3rdparty.system.win32'))
    bld.platforms.append(bld.external('3rdparty.system.dbghelp'))