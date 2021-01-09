def configure(conf):
    conf.env.ARCHITECTURE = 'ppc64le'
    conf.env.VALID_ARCHITECTURES = ['ppc64le', 'powerpc64le']
    conf.env.ARCH_FAMILY = 'ppc'
    conf.env.ARCH_LP64 = True
    conf.env.append_unique('DEFINES', ['_PPC', '_POWERPC', '_PPC64', '_POWERPC64', '_PPC64LE', '_POWERPC64LE', '_LP64'])
