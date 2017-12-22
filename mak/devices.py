import os
import sys
from mak.libs import device
from waflib import Configure, Context, Options, Errors, Logs
from mak.libs.device.command import Command

@Configure.conf
def load_device_list(self):
    self.devices = []
    self.devices.append(device.Device('localhost', 'localhost://'))
    device_dir = os.path.join(Context.top_dir, '.devices')
    try:
        os.mkdir(device_dir, 0o700)
    except OSError:
        pass
    for device_file in os.listdir(device_dir):
        try:
            with open(os.path.join(device_dir, device_file), 'r') as device_file_object:
                name = device_file_object.readline().strip()
                url = device_file_object.readline().strip()
                self.devices.append(device.Device(name, url))
        except IOError:
            Logs.pprint.warn('device file %s could not be opened' % device_file)
        else:
            pass


@Configure.conf
def save_device_list(self):
    device_dir = os.path.join(Context.top_dir, '.devices')
    for d in self.devices:
        with open(os.path.join(device_dir, d.name), 'w') as device_file_object:
            device_file_object.write(d.name)
            device_file_object.write('\n')
            device_file_object.write(d.url)



class DeviceContext(Context.Context):
    cmd = 'device'
    show_status = False

    def __init__(self):
        load_device_list(self)
        Context.Context.__init__(self)
        self.commands = {}
        for cls in Command.__subclasses__():
            self.commands[cls.name] = cls(self.devices)

    def syntax(self, out):
        out.write('python waf device COMMAND [ARGUMENTS]\n')
        out.write('The following commands are understood:\n')
        for name, c in sorted(self.commands.items()):
            c.help(out)

    def run_command(self, commands):
        try:
            c = self.commands[commands[0]]
        except IndexError:
            raise Errors.WafError('Invalid device command syntax')
        except KeyError:
            raise Errors.WafError('Unknwon command %s' % commands[0])
        else:
            try:
                c.run(*commands[1:])
            except TypeError as e:
                #print(e)
                sys.stderr.write('Invalid syntax for command %s\n' % commands[0])
                c.help(sys.stderr)

    def execute(self):
        commands = Options.commands
        Options.commands = []
        try:
            self.run_command(commands)
        except Errors.WafError as e:
            sys.stderr.write('%s\n' % str(e))
            self.syntax(sys.stderr)
            raise
        finally:
            save_device_list(self)


def options(option_context):
    pass

