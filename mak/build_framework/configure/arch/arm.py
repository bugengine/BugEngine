def configure(conf):
    conf.env.ARCHITECTURE = 'arm'
    conf.env.VALID_ARCHITECTURES = ['arm']
    conf.env.ARCH_FAMILY = 'arm'
    conf.env.ARCH_LP64 = False
    conf.env.append_unique('DEFINES', ['_ARM'])
