from . import freebsd, linux, macos, solaris, windows
import os


def _recurse(build_context, fun):
    if build_context.env.VALID_PLATFORMS:
        platform = build_context.env.VALID_PLATFORMS[0]
        try:
            getattr(globals()[platform], fun)(build_context)
        except KeyError:
            build_context.recurse(os.path.join('extra', platform,), fun, once=False)


def build(build_context):
    _recurse(build_context, 'build')


def plugins(build_context):
    _recurse(build_context, 'plugins')


def deploy(build_context):
    _recurse(build_context, 'deploy')

