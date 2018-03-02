from mak.libs.waflib import Errors
from .protocol import Protocol
from .platform import Platform
from copy import copy


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
        try:
            self.protocol_name, self.connection_info = connection.split('://')
        except ValueError:
            raise Errors.WafError('Invalid protocol: %s' % connection)
        else:
            try:
                protocol = self.find_protocol(self.protocol_name)(self.connection_info)
            except KeyError:
                raise Errors.WafError('Unknwon protocol: %s' % self.protocol_name)
        s = self.get_status()
        if s.connection_status != 'online':
            raise Exception('device at address %s is in state %s' % (connection, s.connection_status))
        platform_class = Platform.find_platform(s.platform.lower())
        if not platform_class:
            raise Exception('device at address %s is using unsupported platform %s' % (connection, s.platform))
        self.platform = platform_class(s.all_archs)

    def find_protocol(self, protocol_name):
        for cls in Protocol.__subclasses__():
            if cls.name == protocol_name:
                return cls
        else:
            raise KeyError(protocol_name)

    def get_status(self):
        try:
            try:
                protocol = self.find_protocol(self.protocol_name)(self.connection_info)
            except KeyError:
                raise Errors.WafError('Unknwon protocol: %s' % self.protocol_name)
            else:
                with protocol.connect() as c:
                    return Status(*(c.status() + ('',)))
        except Exception as e:
            return Status('error', None, None, None, str(e))

    def connect(self):
        try:
            protocol = self.find_protocol(self.protocol_name)(self.connection_info)
        except KeyError:
            raise Errors.WafError('Unknwon protocol: %s' % self.protocol_name)
        else:
            return protocol.connect()

    def __getstate__(self):
        result = copy(self.__dict__)
        del result['platform']
        result['platform_name'] = self.platform.__class__.__name__
        result['platform_archs'] = self.platform.architectures
        return result

    def __setstate__(self, state):
        self.__dict__ = state
        platform_class = Platform.find_platform(self.platform_name.lower())
        if not platform_class:
            raise Exception('device %s is using unsupported platform %s' % (self.name, self.platform_name))
        else:
            self.platform = platform_class(self.platform_archs)
