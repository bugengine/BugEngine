def build(bld):
    if not bld.env.SYSTEM_SQUIRREL or bld.env.PROJECTS:
        bld.static_library('3rdparty.squirrel', bld.platforms,
                           path='.',
                           warnings=False)
