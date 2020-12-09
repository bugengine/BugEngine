# set iPhone(Simulator) build rules
from waflib.TaskGen import feature, after_method
import os


def build(build_context):
    build_context.recurse(
        os.path.join(
            build_context.bugenginenode.abspath(), 'mak', 'libs', 'build_framework', 'build', 'target', 'macos.py'
        )
    )


@feature('cprogram', 'cxxprogram')
@after_method('apply_link')
@after_method('process_use')
def set_iphone_program_name(self):
    pass
