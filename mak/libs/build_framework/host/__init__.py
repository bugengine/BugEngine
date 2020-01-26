import platform
from . import darwin, freebsd, linux, sunos, windows

os_name = platform.uname()[0].lower().split('-')[0]


def options(option_context):
    globals()[os_name].options(option_context)


def configure(configuration_context):
    configuration_context.env.HOST = os_name
    globals()[os_name].configure(configuration_context)


def build(build_context):
    globals()[os_name].build(build_context)


def plugins(build_context):
    globals()[os_name].plugins(build_context)
