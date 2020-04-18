# set iPhone(Simulator) configuration specific options
from waflib import Configure


class iOS(Configure.ConfigurationContext.Darwin):
    NAME = 'iPhone'
    CERTIFICATE_NAME = 'iPhone Developer'
    PLATFORMS = ['iphone', 'darwin']
    SDK_NAME = 'iPhoneOS'
    OS_NAME = 'iphoneos'

    def __init__(self, conf, sdk = None):
        Configure.ConfigurationContext.Darwin.__init__(self, conf, sdk)

    def get_root_dirs(self, appname):
        return (appname + '.app',
                appname + '.app')


    def load_in_env(self, conf, compiler):
        Configure.ConfigurationContext.Darwin.load_in_env(self, conf, compiler)
        if 'GCC' in compiler.NAMES:
            conf.env.append_unique('LINKFLAGS_cxxshlib', ['-lgcc_eh'])
            conf.env.append_unique('LINKFLAGS_cxxprogram', ['-lgcc_eh'])

class iOSSimulator(iOS):
    NAME = 'iPhoneSimulator'
    PLATFORMS = ['iphone', 'iphonesimulator']
    SDK_NAME = 'iPhoneSimulator'
    OS_NAME = 'iphoneos'

    def __init__(self, conf, sdk = None):
        Configure.ConfigurationContext.Darwin.__init__(self, conf, sdk)


def configure(conf):
    conf.platforms.append(iOS(conf))
    conf.platforms.append(iOSSimulator(conf))

