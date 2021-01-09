from waflib.TaskGen import feature


@feature('cxxprogram')
def set_linux_rpath_program(task_gen):
    if 'linux' in task_gen.env.VALID_PLATFORMS:
        task_gen.env.RPATH = [
            ':'.join(
                ['$ORIGIN', '$ORIGIN/../lib/', '$ORIGIN/../lib/bugengine/'] +
                ['$ORIGIN/../lib/%s' % target for target in task_gen.env.TARGETS] +
                ['$ORIGIN/../lib/%s/bugengine' % target for target in task_gen.env.TARGETS]
            )
        ]


@feature('cxxshlib')
def set_linux_rpath_cxxshlib(task_gen):
    if 'linux' in task_gen.env.VALID_PLATFORMS:
        if 'bugengine:kernel' in task_gen.features or 'bugengine:plugin' in task_gen.features:
            task_gen.env.RPATH = ['$ORIGIN:$ORIGIN/../']
        else:
            task_gen.env.RPATH = [':'.join(['$ORIGIN', '$ORIGIN/bugengine/'])]


def build(bld):
    pass