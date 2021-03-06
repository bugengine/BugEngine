def configure(conf):
    conf.recurse('armv7a.py', once=False)
    conf.env.VALID_ARCHITECTURES = ['armv7s'] + conf.env.VALID_ARCHITECTURES
    conf.env.append_unique('DEFINES', ['_ARM_V7S'])
