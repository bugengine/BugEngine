def configure(conf):
    conf.recurse('armv7.py', once=False)
    conf.env.VALID_ARCHITECTURES = ['armv7a'] + conf.env.VALID_ARCHITECTURES
    conf.env.append_unique('DEFINES', ['_ARM_V7A'])
