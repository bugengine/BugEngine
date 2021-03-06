def configure(conf):
    conf.env.ARCHITECTURE = 'ppc'
    conf.env.VALID_ARCHITECTURES = ['ppc', 'powerpc']
    conf.env.ARCH_FAMILY = 'ppc'
    conf.env.ARCH_LP64 = False
    conf.env.append_unique('DEFINES', ['_PPC', '_POWERPC'])
