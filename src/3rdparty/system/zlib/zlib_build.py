from waflib import Options


def build(bld):
    if not bld.env.SYSTEM_ZLIB or Options.options.included_zlib or bld.env.PROJECTS:
        bld.shared_library('3rdparty.system.zlib', bld.platforms,
                           path='.', use_master=False,
                           extra_defines=['ZLIB_DLL', 'ZLIB_INTERNAL'],
                           extra_public_defines=['ZLIB_DLL'],
                           warnings=False, export_all=True)
    else:
        bld.thirdparty('3rdparty.system.zlib')