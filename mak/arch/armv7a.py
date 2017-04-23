def configure(conf):
    conf.recurse('armv7', once=False)
    conf.env.VALID_ARCHITECTURES.append('armv7a')
    conf.env.append_unique('DEFINES', ['_ARM_V7A'])
