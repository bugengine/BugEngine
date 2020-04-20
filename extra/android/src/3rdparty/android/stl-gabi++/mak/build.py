def build(bld):
    libs, multiarch = bld.static_library('3rdparty.android.stl-gabi++',
                                         path='.', warnings=False)
    for l in libs:
        l.env.append_value('CXXFLAGS', ['-std=c++98'])
        l.env.append_value('CXXFLAGS_profile', l.env.CXXFLAGS_rtti)
        l.env.append_value('CXXFLAGS_profile', l.env.CXXFLAGS_exception)
        l.env.append_value('CXXFLAGS_final', l.env.CXXFLAGS_rtti)
        l.env.append_value('CXXFLAGS_final', l.env.CXXFLAGS_exception)
