from . import armv7


def configure(conf):
    armv7.configure(conf)
    conf.env.VALID_ARCHITECTURES = ['armv7a'] + conf.env.VALID_ARCHITECTURES
    conf.env.append_unique('DEFINES', ['_ARM_V7A'])
