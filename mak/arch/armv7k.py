def configure(conf):
    conf.recurse('armv7a', once=False)
    conf.env.VALID_ARCHITECTURES.append('armv7k')
    conf.env.append_unique('DEFINES', ['_ARM_V7K'])
