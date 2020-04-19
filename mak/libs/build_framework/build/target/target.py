import os


def _recurse(build_context, fun):
    if build_context.env.VALID_PLATFORMS:
        platform = build_context.env.VALID_PLATFORMS[0]
        if os.path.isfile(os.path.join(build_context.path.abspath(), platform + '.py')):
            build_context.recurse(platform + '.py', fun, once=False)
        else:
            build_context.recurse(os.path.join(build_context.bugenginenode.abspath(),
                                               'extra', platform,),
                                  fun, once=False)


def build(build_context):
    _recurse(build_context, 'build')


def deploy(build_context):
    _recurse(build_context, 'deploy')

