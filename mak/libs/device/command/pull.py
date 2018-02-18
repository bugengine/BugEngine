import os
from . import Command

class PullCommand(Command):
    name = 'pull'

    def run(self, device, device_file, destination):
        for d in self.all_devices():
            if d.name == device:
                with d.connect() as connection:
                    connection.pull(device_file, destination)
                break
        else:
            raise Exception('Unknown device %s' % device)

    @classmethod
    def help(self, out):
        out.write('    %s device file destination\n' % self.bold_name())
        out.write('        Downloads a file from the device\n')

