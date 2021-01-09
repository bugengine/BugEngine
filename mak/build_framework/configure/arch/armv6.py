def configure(conf):
    conf.recurse('arm.py', once=False)
    conf.env.VALID_ARCHITECTURES = ['armv6'] + conf.env.VALID_ARCHITECTURES
    conf.env.append_unique('DEFINES', ['_ARM_V6'])
