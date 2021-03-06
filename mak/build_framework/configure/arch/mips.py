def configure(conf):
    conf.env.ARCHITECTURE = 'mips'
    conf.env.VALID_ARCHITECTURES = ['mips']
    conf.env.ARCH_FAMILY = 'mips'
    conf.env.ARCH_LP64 = False
    conf.env.append_unique('DEFINES', ['_MIPS'])
