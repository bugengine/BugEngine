import sys
from . import Command
from .. import Device
from ..protocol import Protocol


class AddCommand(Command):
    name = 'add'

    def run(self, name, url):
        print('registering device %s (%s)' % (name, url))
        device = Device(name, url)
        try:
            self.devices[device.platform.__class__.__name__].append(device)
        except KeyError:
            self.devices[device.platform.__class__.__name__] = [device]

    @classmethod
    def help(self, out):
        out.write('    %s <name> <protocol>://[protocol_parameters]\n' % self.bold_name())
        out.write('        Adds a device connected through the specified protocol\n')

