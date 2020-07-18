from waflib import Task, Utils, Errors
from waflib.TaskGen import feature, before_method, after_method, extension
from waflib.Tools import msvc
import os
import re
import coff


class symbols(Task.Task):
    """
    extract all symbols
    """
    color = 'GREEN'

    def run(self):
        with open(self.outputs[0].abspath(), 'w') as export_file:
            if self.generator.env.CC_NAME != 'msvc':
                export_file.write('asm (".section .drectve");\n')
            seen = set([])
            for input in self.inputs:
                coff_file = coff.Coff(input.abspath())
                for table in coff_file.symtables.items():
                    for symbol in table[1]:
                        if symbol.storagecls == coff.IMAGE_SYM_CLASS_EXTERNAL and symbol.value != 0 and symbol.name not in seen:
                            seen.add(symbol.name)
                            export_file.write('/* %s */\n' % (str(symbol)))
                            if self.generator.env.CC_NAME != 'msvc':
                                if self.generator.env.ARCHITECTURE == 'x86':
                                    undecorated_name = symbol.name[1:]
                                else:
                                    undecorated_name = symbol.name
                                export_file.write('asm (".ascii \\" -export:%s\\"");\n' % (undecorated_name))
                            else:
                                export_file.write('#pragma comment(linker, "/export:%s")\n' % (symbol.name))


@feature('bugengine:export_all')
@after_method('process_source')
@before_method('apply_link')
def generate_export_file(self):
    if self.env.DEST_BINFMT == 'pe':
        export_file = self.make_bld_node('obj', '', 'exports.cc')
        self.create_task('symbols', [c.outputs[0] for c in self.compiled_tasks], [export_file])
        self.create_compiled_task('cxx', export_file)


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