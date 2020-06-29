def build(bld):
    bld.static_library('3rdparty.android.rand', path=bld.path.parent)
