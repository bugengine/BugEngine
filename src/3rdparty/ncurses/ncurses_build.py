def build(bld):
    if 'windows' in bld.env.VALID_PLATFORMS or bld.env.PROJECTS:
        includes = [bld.path.make_node('src/ncurses')]
        bld.static_library('3rdparty.ncurses', path='.', extra_includes=includes,
                           warnings=False, use_master=False)
        bld.env.append_unique('FEATURES', ['ncurses'])
    else:
        for env in bld.multiarch_envs:
            if env.NCurses:
                bld.thirdparty('3rdparty.ncurses', '.', env, libs=env.NCurses)
                env.append_unique('FEATURES', ['ncurses'])
