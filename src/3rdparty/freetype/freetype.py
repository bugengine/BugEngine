def options(opt):
    pass

def setup(conf):
    if 'posix' in conf.env.VALID_PLATFORMS:
        try:
            conf.pkg_config('freetype2', var='3rdparty.freetype')
        except Exception:
            pass
        else:
            conf.env.SYSTEM_FREETYPE = True
