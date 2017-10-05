from waflib import Options


def build(bld):
    if not bld.env.SYSTEM_FREETYPE or Options.options.included_freetype or bld.env.PROJECTS:
        defines = ['FT2_BUILD_LIBRARY=1',
                   'FT_CONFIG_OPTION_SYSTEM_ZLIB=1',
                   'FT_CONFIG_OPTION_NO_ASSEMBLER']
        include_path = bld.path.find_node('include')
        includes = []
        for i in include_path.listdir():
            includes.append(include_path.find_node(i))
        bld.static_library('3rdparty.system.freetype', ['3rdparty.system.zlib'],
                           path='.',
                           extra_includes=includes,
                           extra_defines=defines,
                           use_master=False,
                           warnings=False)
    else:
        bld.thirdparty('3rdparty.system.freetype')
