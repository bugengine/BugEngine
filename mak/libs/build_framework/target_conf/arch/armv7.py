from . import arm


def configure(conf):
    arm.configure(conf)
    conf.env.VALID_ARCHITECTURES = ['armv7'] + conf.env.VALID_ARCHITECTURES
    conf.env.append_unique('DEFINES', ['_ARM_V7'])