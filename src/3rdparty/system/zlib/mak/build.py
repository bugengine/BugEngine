from waflib import Options


def build(bld):
    if not bld.env.SYSTEM_ZLIB or Options.options.included_zlib or bld.env.PROJECTS:
        dll_defs = (not bld.env.STATIC) and ['ZLIB_DLL'] or []
        bld.shared_library('3rdparty.system.zlib', bld.platforms,
                           path=bld.path.parent, use_master=False,
                           extra_defines=dll_defs + ['ZLIB_INTERNAL'],
                           extra_public_defines=dll_defs,
                           warnings=False, export_all=True)
    else:
        bld.thirdparty('3rdparty.system.zlib')
