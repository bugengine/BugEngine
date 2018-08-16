# set iPhone(Simulator) configuration specific options
from waflib import Configure


class iOS(Configure.ConfigurationContext.Darwin):
    NAME = 'iPhone'
    PLATFORMS = ['iphone', 'darwin']
    SDK_NAME = 'iPhoneOS'
    OS_NAME = 'iphoneos'

    def __init__(self, conf, sdk = None):
        Configure.ConfigurationContext.Darwin.__init__(self, conf, sdk)

    def get_root_dirs(self, appname):
        return (appname + '.app',
                appname + '.app')


class iOSSimulator(iOS):
    NAME = 'iPhoneSimulator'
    PLATFORMS = ['iphone', 'iphonesimulator']
    SDK_NAME = 'iPhoneSimulator'
    OS_NAME = 'iphoneos'

    def __init__(self, conf, sdk = None):
        Configure.ConfigurationContext.Darwin.__init__(self, conf, sdk)


def host_configure(conf):
    pass


def configure(conf):
    conf.platforms.append(iOS(conf))
    conf.platforms.append(iOSSimulator(conf))


def setup(conf):
    pass

