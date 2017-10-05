from waflib import Options


def build(bld):
    if not bld.env.SYSTEM_MINIZIP or Options.options.included_minizip or bld.env.PROJECTS:
        bld.static_library('3rdparty.system.minizip', bld.platforms + ['3rdparty.system.zlib'],
                           path='.', use_master = True, warnings=False)
    else:
        bld.thirdparty('3rdparty.system.minizip')
