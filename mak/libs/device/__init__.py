from mak.libs.waflib import Errors
from .protocol import Protocol
from .platform import Platform


class Status:
    def __init__(self, connection_status, platform_name, arch_name, all_archs, message):
        self.connection_status = connection_status
        self.platform = platform_name
        self.arch = arch_name
        self.all_archs = all_archs
        self.message = message


class Device:
    def __init__(self, name, connection):
        self.name = name
        self.url = connection
        try:
            protocol, connection_info = connection.split('://')
        except ValueError:
            raise Errors.WafError('Invalid protocol: %s' % connection)
        else:
            try:
                self.protocol = self.find_protocol(protocol)(connection_info)
            except KeyError:
                raise Errors.WafError('Unknwon protocol: %s' % protocol)

    def find_protocol(self, protocol_name):
        for cls in Protocol.__subclasses__():
            if cls.name == protocol_name:
                return cls
        else:
            raise KeyError(protocol_name)

    def get_status(self):
        try:
            with self.protocol.connect() as c:
                return Status(*(c.status() + ('',)))
        except Exception as e:
            return Status('error', None, None, None, str(e))

    def connect(self):
        return self.protocol.connect()
