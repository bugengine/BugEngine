def build(bld):
    bld.static_library('3rdparty.squirrel', bld.platforms,
                       path='.',
                       warnings=False)
