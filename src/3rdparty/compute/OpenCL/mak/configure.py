def configure(configuration_context):
    configuration_context.start_msg('Looking for clang 10+')
    v = configuration_context.env
    for c in configuration_context.compilers:
        if 'Clang' in c.NAMES:
            if c.version_number >= (10, ):
                v.CLC_CXX = c.compiler_cxx
    if v.CLC_CXX:
        v.CLC_CXXFLAGS_debug = ['-D_DEBUG']
        v.CLC_CXXFLAGS_profile = ['-DNDEBUG', '-fno-rtti', '-fno-exceptions']
        v.CLC_CXXFLAGS_final = ['-DNDEBUG', '-fno-rtti', '-fno-exceptions']
        v.CLC_CXXFLAGS = ['-std=cl1.2']
        v.CLC_CXX_SRC_F = ''
        v.CLC_CXX_TGT_F = ['-o']
        v.CLC_ARCH_ST = ['-arch']
        v.CLC_FRAMEWORKPATH_ST = ['-F%s']
        v.CLC_FRAMEWORK_ST = ['-framework']
        v.CLC_CPPPATH_ST = ['-I']
        v.CLC_DEFINES_ST = ['-D']
        v.CLC_CXXFLAGS_warnnone = ['-w']
        v.CLC_CXXFLAGS_warnall = [
            '-Wall', '-Wextra', '-Wno-invalid-offsetof', '-Werror', '-Wno-sign-compare', '-Woverloaded-virtual',
            '-Wstrict-aliasing'
        ]
    configuration_context.end_msg(configuration_context.env.CLC_CXX)
