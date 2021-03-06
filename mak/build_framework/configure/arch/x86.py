def configure(conf):
    conf.env.ARCHITECTURE = 'x86'
    conf.env.VALID_ARCHITECTURES = ['x86', 'i386']
    conf.env.ARCH_FAMILY = 'x86'
    conf.env.ARCH_LP64 = False
    conf.env.append_unique('DEFINES', '_X86')
