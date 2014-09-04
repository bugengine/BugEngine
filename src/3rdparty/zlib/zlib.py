def options(opt):
    pass

def setup(conf):
    if 'posix' in conf.env.VALID_PLATFORMS:
        try:
            conf.pkg_config('zlib', var='3rdparty.zlib')
        except Exception:
            pass
        else:
            conf.env.SYSTEM_ZLIB = True
