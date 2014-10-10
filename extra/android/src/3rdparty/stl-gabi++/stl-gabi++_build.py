def build(bld):
    stdcpp = bld.static_library('3rdparty.stl-gabi++', path='.', warnings=False)
