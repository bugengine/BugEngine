# set iPhone(Simulator) build rules
from waflib.TaskGen import feature, after_method
import os
from build_framework.target_build.macos


def build(build_context):
    macos.build(build_context)


def plugins(bld):
    macos.plugins(build_context)


@feature('cxxshlib', 'cshlib')
@after_method('apply_link')
def set_iphone_shlib_name(self):
    if 'iphone' in self.env.VALID_PLATFORMS:
        if 'plugin' in self.features:
            self.env.append_unique('LINKFLAGS', ['-install_name', os.path.join('@executable_path', 'share', 'bugengine', 'plugin', self.link_task.outputs[0].name)])
        elif 'kernel' in self.features:
            self.env.append_unique('LINKFLAGS', ['-install_name', os.path.join('@executable_path', 'share', 'bugengine', 'kernel', self.link_task.outputs[0].name)])
        else:
            self.env.append_unique('LINKFLAGS', ['-install_name', os.path.join('@loader_path', self.link_task.outputs[0].name)])


@feature('cprogram', 'cxxprogram')
@after_method('apply_link')
@after_method('process_use')
def set_iphone_program_name(self):
    pass

