class Platform(object):
    def __init__(self, architectures):
        self.architectures = architectures

    @classmethod
    def help(self, out):
        out.write('    supports %s as a target\n' % (self.__name__))

    @classmethod
    def find_platform(self, name):
        for subclass in self.__subclasses__():
            if name in (subclass.__name__.lower(),) + subclass.names:
                return subclass

    @classmethod
    def list_devices(self):
        return []

from .freebsd import FreeBSD
from .linux import Linux
from .macos import MacOS
from .solaris import Solaris
from .windows import Windows
