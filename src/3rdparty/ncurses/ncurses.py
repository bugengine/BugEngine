
def options(opt):
    pass

def setup(conf):
    if not 'windows' in conf.env.VALID_PLATFORMS:
        conf.check_lib(['ncurses', 'panel', 'form', 'menu'], var='NCurses')

def build(bld):
    if 'windows' in bld.env.VALID_PLATFORMS or bld.env.PROJECTS:
        bld.static_library('3rdparty.ncurses', warnings=False, use_master=False)
        bld.env.append_unique('FEATURES', ['ncurses'])
    else:
        for env in bld.multiarch_envs:
            if env.NCurses:
                bld.thirdparty('3rdparty.ncurses', env, libs=env.NCurses)
                env.append_unique('FEATURES', ['ncurses'])
