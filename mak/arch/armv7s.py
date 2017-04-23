def configure(conf):
    conf.recurse('armv7a', once=False)
    conf.env.VALID_ARCHITECTURES.append('armv7s')
    conf.env.append_unique('DEFINES', ['_ARM_V7S'])
