import platform


def build(build_context):
    os_name = platform.uname()[0].lower().split('-')[0]
    build_context.recurse(os_name)
