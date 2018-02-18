import os
import sys
from mak.libs import device
from waflib import Configure, Context, Options, Errors, Logs
from mak.libs.device.command import Command
try:
    import cPickle as pickle
except ImportError:
    import pickle


@Configure.conf
def load_device_list(self):
    self.devices = {}
    device_dir = os.path.join(Context.top_dir, '.devices')
    try:
        os.mkdir(device_dir, 0o700)
    except OSError:
        pass
    localhost = False
    for device_file in os.listdir(device_dir):
        try:
            with open(os.path.join(device_dir, device_file), 'rb') as device_file_object:
                d = pickle.load(device_file_object)
        except Exception as e:
            Logs.warn('device file %s could not be opened: %s' % (device_file, e))
        except IOError:
            Logs.warn('device file %s could not be opened' % device_file)
        else:
            try:
                self.devices[d.platform.__class__.__name__].append(d)
            except KeyError:
                self.devices[d.platform.__class__.__name__] = [d]
            localhost = localhost or (d.name == 'localhost')
    if not localhost:
        localhost = device.Device('localhost', 'localhost://')
        try:
            self.devices[localhost.platform.__class__.__name__].append(localhost)
        except KeyError:
            self.devices[localhost.platform.__class__.__name__] = [localhost]


@Configure.conf
def save_device_list(self):
    device_dir = os.path.join(Context.top_dir, '.devices')
    for platform, devices in self.devices.items():
        for d in devices:
            try:
                with open(os.path.join(device_dir, d.name), 'wb') as device_file_object:
                    pickle.dump(d, device_file_object)
            except pickle.PicklingError:
                os.unlink(os.path.join(device_dir, d.name))
                raise


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
    option_context.add_option('--device',
                              action='store',
                              default='clamps',
                              dest='device',
                              help='Default device to deploy to')

