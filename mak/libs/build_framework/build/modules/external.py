from waflib.Configure import conf
import os


@conf
def external(bld, name):
    namespace = name.split('.')
    script_file = os.path.join('/'.join(namespace), 'mak', 'build.py')
    if os.path.isfile(os.path.join(bld.path.abspath(), script_file)):
        bld.recurse(script_file)
    else:
        bld.recurse(os.path.join(bld.bugenginenode.abspath(), 'src', script_file))
    return name


def build(build_context):
    pass