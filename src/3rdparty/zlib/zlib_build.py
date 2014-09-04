def build(bld):
    if not bld.env.SYSTEM_ZLIB:
        bld.shared_library('3rdparty.zlib', bld.platforms, use_master=False,
                           extra_defines=['ZLIB_DLL'], extra_public_defines=['ZLIB_DLL'],
                           warnings=False, export_all=True)
