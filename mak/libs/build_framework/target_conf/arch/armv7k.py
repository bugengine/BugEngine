from . import armv7a


def configure(conf):
    armv7a.configure(conf)
    conf.env.VALID_ARCHITECTURES = ['armv7k'] + conf.env.VALID_ARCHITECTURES
    conf.env.append_unique('DEFINES', ['_ARM_V7K'])
