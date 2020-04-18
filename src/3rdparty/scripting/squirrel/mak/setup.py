from waflib.Logs import pprint


def setup(conf):
    try:
        conf.pkg_config('squirrel3', var='squirrel')
    except Exception as e:
        include = 'squirrel.h'
        if conf.check_lib('squirrel',
                          includes=[include],
                          functions=['sq_open']):
            conf.env.SYSTEM_SQUIRREL = True
            pprint('GREEN', '+squirrel', sep=' ')
        else:
            pprint('BLUE', '=squirrel', sep=' ')
    else:
        conf.env.SYSTEM_SQUIRREL = True
        pprint('GREEN', '+squirrel', sep=' ')
