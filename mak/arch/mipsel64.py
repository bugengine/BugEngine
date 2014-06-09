def configure(conf):
    conf.env.ARCHITECTURE = 'mipsel64'
    conf.env.VALID_ARCHITECTURES = ['mipsel64']
    conf.env.ARCH_FAMILY = 'mips'
    conf.env.ARCH_LP64 = True
    conf.env.append_unique('DEFINES', ['_MIPS', '_MIPS64', '_MIPSEL', '_MIPSEL64'])