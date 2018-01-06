
def build(bld):
    bld.platforms.append(bld.external('3rdparty.system.win32'))
    bld.platforms.append(bld.external('3rdparty.system.dbghelp'))


def plugins(bld):
    pass
