class Protocol(object):
    @classmethod
    def find_protocol(self, name):
        for subclass in self.__subclasses__():
            if subclass.name == name:
                return subclass


from .localhost import LocalhostProtocol
from .ssh import SSHProtocol
