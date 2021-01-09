import platform


def options(option_context):
    os_name = platform.uname()[0].lower().split('-')[0]
    option_context.recurse(os_name)
