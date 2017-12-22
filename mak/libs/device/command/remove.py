import sys
from . import Command


class RemoveCommand(Command):
    name = 'remove'

    def __init__(self, devices):
        self.devices = devices

    def run(self, name):
        print('unregistering device %s' % name)

    @classmethod
    def help(self, out):
        out.write('    %s name\n' % self.bold_name())
        out.write('        Removes the device by name\n')

