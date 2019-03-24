def configure(conf):
    conf.recurse('armv7a', once=False)
    conf.env.VALID_ARCHITECTURES = ['armv7k'] + conf.env.VALID_ARCHITECTURES
    conf.env.append_unique('DEFINES', ['_ARM_V7K'])
