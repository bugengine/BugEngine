import sys
from . import Command
from .. import Device
from ..protocol import Protocol


class AddCommand(Command):
    name = 'add'

    def __init__(self, devices):
        self.devices = devices

    def run(self, name, url):
        print('registering device %s (%s)' % (name, url))
        self.devices.append(Device(name, url))

    @classmethod
    def help(self, out):
        out.write('    %s name protocol://[protocol_parameters]\n' % self.bold_name())
        out.write('        Adds a device connected through the specified protocol\n')

