from . import freebsd, linux, macos, solaris, windows
import os


def build(build_context):
    platform = build_context.env.VALID_PLATFORMS[0]
    try:
        globals()[platform].build(build_context)
    except KeyError:
        build_context.recurse(os.path.join('extra', platform))


def plugins(build_context):
    platform = build_context.env.VALID_PLATFORMS[0]
    try:
        globals()[platform].plugins(build_context)
    except KeyError:
        build_context.recurse(os.path.join('extra', platform))


def deploy(build_context):
    platform = build_context.env.VALID_PLATFORMS[0]
    try:
        globals()[platform].deploy(build_context)
    except KeyError:
        build_context.recurse(os.path.join('extra', platform))


