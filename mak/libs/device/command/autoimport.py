import sys
from . import Command
from ..platform import Platform


class AutoimportCommand(Command):
    name = 'autoimport'

    def run(self):
        print('importing all discoverable devices')
        existing_device_list = list(self.all_devices())
        for platform in Platform.__subclasses__():
            try:
                for device in platform.list_devices():
                    for existing_device in self.all_devices():
                        if existing_device.matches(device):
                            print('skipping import of already registered device at address %s://%s' % (device.protocol_name, device.connection_info))
                            break
                    else:
                        print('importing %s at address %s://%s' % (device.name, device.protocol_name, device.connection_info))
                        try:
                            self.devices[device.platform.__class__.__name__].append(device)
                        except KeyError:
                            self.devices[device.platform.__class__.__name__] = [device]
            except Exception as e:
                print(e)
                raise

    @classmethod
    def help(self, out):
        out.write('    %s\n' % self.bold_name())
        out.write('        Automatically import all discoverable devices\n')
