def build(bld):
    bld.static_library('3rdparty.squirrel', bld.platforms,
                       path='.',
                       extra_defines=['_BSD_SOURCE'], warnings=False)
