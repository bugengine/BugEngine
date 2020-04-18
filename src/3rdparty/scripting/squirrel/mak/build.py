from waflib import Options


def build(bld):
    if not bld.env.SYSTEM_SQUIRREL or Options.options.included_squirrel or bld.env.PROJECTS:
        bld.static_library('3rdparty.scripting.squirrel', bld.platforms,
                           path=bld.path.parent,
                           warnings=False)
    else:
        bld.thirdparty('3rdparty.scripting.squirrel')
