def configure(conf):
    conf.recurse('arm', once=False)
    conf.env.VALID_ARCHITECTURES = ['armv7'] + conf.env.VALID_ARCHITECTURES
    conf.env.append_unique('DEFINES', ['_ARM_V7'])
