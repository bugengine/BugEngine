def configure(conf):
    conf.env.ARCHITECTURE = 'arm64_32'
    conf.env.VALID_ARCHITECTURES = ['arm64_32', 'aarch64_32']
    conf.env.ARCH_FAMILY = 'arm'
    conf.env.ARCH_LP64 = False
    conf.env.append_unique('DEFINES', ['_ARM', '_ARM64', '_AARCH64', '_ILP32'])
