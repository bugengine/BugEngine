from waflib import Options


def build(bld):
    if not bld.env.SYSTEM_ZLIB or Options.options.included_zlib or bld.env.PROJECTS:
        dll_defs = (not bld.env.STATIC) and ['ZLIB_DLL'] or []
        bld.shared_library(
            '3rdparty.system.zlib',
            bld.platforms,
            path=bld.path.parent,
            features=['bugengine:warnings:off', 'bugengine:masterfiles:off'],
            extra_defines=dll_defs,
            extra_public_defines=dll_defs
        )
    else:
        bld.thirdparty('3rdparty.system.zlib')
