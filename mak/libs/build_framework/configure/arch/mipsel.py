def configure(conf):
    conf.env.ARCHITECTURE = 'mipsel'
    conf.env.VALID_ARCHITECTURES = ['mipsel']
    conf.env.ARCH_FAMILY = 'mips'
    conf.env.ARCH_LP64 = False
    conf.env.append_unique('DEFINES', ['_MIPS', '_MIPSEL'])
