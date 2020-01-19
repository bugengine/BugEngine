def configure(conf):
    conf.env.ARCHITECTURE = 'arm64'
    conf.env.VALID_ARCHITECTURES = ['arm64', 'aarch64']
    conf.env.ARCH_FAMILY = 'arm'
    conf.env.ARCH_LP64 = True
    conf.env.append_unique('DEFINES', ['_ARM', '_ARM64', '_AARCH64', '_LP64'])
