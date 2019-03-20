import sys
from . import Command
from .. import Device
from ..protocol import Protocol


class BootCommand(Command):
    name = 'boot'

    def run(self, device_name):
        print('booting device %s' % (device_name))
        for device in self.all_devices():
            if device.name == device_name:
                try:
                    with device.connect() as connection:
                        connection.boot()
                except AttributeError as e:
                    raise Exception("The device %s can't be booted remotely" % device_name)
                except Exception as e:
                    print(e)
                break
        else:
            raise Exception('Unknown device %s' % device_name)


    @classmethod
    def help(self, out):
        out.write('    %s <device_name>\n' % self.bold_name())
        out.write('        Boots the registered device\n')



class ShutdownCommand(Command):
    name = 'shutdown'

    def run(self, device_name):
        print('shutting down device %s' % (device_name))
        for device in self.all_devices():
            if device.name == device_name:
                try:
                    with device.connect() as connection:
                        connection.shutdown()
                except AttributeError as e:
                    raise Exception("The device %s can't be shut down remotely" % device_name)
                except Exception as e:
                    print(e)
                break
        else:
            raise Exception('Unknown device %s' % device_name)


    @classmethod
    def help(self, out):
        out.write('    %s <device_name>\n' % self.bold_name())
        out.write('        Shuts down the registered device\n')

