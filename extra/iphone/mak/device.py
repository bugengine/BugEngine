import os
import sys
import subprocess
import json
from build_framework.device.protocol import Protocol
from build_framework.device.platform import Platform
from build_framework.device.device import Device
from waflib import Options

class SimctlProtocol(Protocol):
    name = 'iossim'

    class Connection:
        def __init__(self, device_name):
            if Options.options.xcode_sdks:
                self.simctl = [os.path.join(Options.options.xcode_sdks.split(',')[0], 'usr', 'bin','simctl')]
            else:
                self.simctl = ['xcrun', 'simctl']
            self.device_name = device_name

        def __enter__(self):
            return self

        def __exit__(self, type, value, traceback):
            pass

        def _run(self, command):
            p = subprocess.Popen(self.simctl + command,
                                 stdin=subprocess.PIPE,
                                 stdout=subprocess.PIPE,
                                 stderr=subprocess.PIPE)
            out, err = p.communicate()
            if p.returncode:
                raise Exception('error connecting to device %s' % (self.device_name))
            if not isinstance(out, str):
                out = out.decode(sys.stdout.encoding, errors='ignore')
            if not isinstance(err, str):
                err = err.decode(sys.stderr.encoding, errors='ignore')
            return out, err

        def status(self):
            out, err = self._run(['list', '-j', 'devices'])
            devices = json.loads(out)
            for sdk, device_list in devices['devices'].items():
                for device in device_list:
                    if device['udid'] == self.device_name:
                        if device['state'] == 'Booted':
                            return ('online', 'ios-simulator', 'x86_64', ['x86_64', 'i386'])
                        elif device['state'] == 'Shutdown':
                            return ('standby', 'ios-simulator', 'x86_64', ['x86_64', 'i386'])
                        else:
                            raise Exception('Unknown state: %s' % device['state'])

        def mkdir(self, dir):
            pass

        def put(self, file, destination):
            pass

        def pull(self, file, destination):
            pass

        def start_shell(self):
            raise Exception('shell not supported on iOS devices')

        def install(self, package_file):
            pass

        def start_activity(self, task_gen):
            pass

        def boot(self):
            out, err = self._run(['boot', self.device_name])
            print(out)

        def shutdown(self):
            out, err = self._run(['shutdown', self.device_name])
            print(out)

        def list_devices(self):
            result = []
            try:
                out, err = self._run(['list', '-j', 'devices'])
                devices = json.loads(out)
            except Exception as e:
                return result
            else:
                for sdk, device_list in devices['devices'].items():
                    for ios_device in device_list:
                        result.append(Device(ios_device['name'], 'iossim://%s' % ios_device['udid']))
                return result

    def __init__(self, parameters):
        self.device_name = parameters

    def connect(self):
        return SimctlProtocol.Connection(self.device_name)

    @classmethod
    def help(self, out):
        out.write('    Connects to an iOS Simulator device.\n')
        out.write('    Syntax: %s://device_uid\n' % self.name)

class iOSSimulatorPlatform(Platform):
    name = 'iOS Simulator'
    names = ('ios-simulator',)

    @classmethod
    def help(self, out):
        out.write('    supports iOS simulator devices through XCode simctl\n')

    @classmethod
    def list_devices(self):
        with SimctlProtocol('dummy').connect() as connection:
            return connection.list_devices()

def options(option_context):
    pass
