def configure(conf):
    conf.recurse('arm', once=False)
    conf.env.VALID_ARCHITECTURES.append('armv7')
    conf.env.append_unique('DEFINES', ['_ARM_V7'])
