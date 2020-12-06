def configure(conf):
    conf.env.ARCHITECTURE = 'aarch32'
    conf.env.VALID_ARCHITECTURES = ['aarch32', 'aarch64']
    conf.env.ARCH_FAMILY = 'arm'
    conf.env.ARCH_LP64 = True
    conf.env.append_unique('DEFINES', ['_ARM', '_ARM64', '_AARCH64', '_AARCH32', '_LP64'])
