def configure(conf):
    conf.env.ARCHITECTURE = 'arm64e'
    conf.env.VALID_ARCHITECTURES = ['arm64e', 'arm64', 'aarch64']
    conf.env.ARCH_FAMILY = 'arm'
    conf.env.ARCH_LP64 = True
    conf.env.append_unique('DEFINES', ['_ARM', '_ARM64', '_ARM64E', '_AARCH64', '_LP64'])
