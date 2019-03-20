import os
import sys
import subprocess
from mak.libs.device.protocol import Protocol
from mak.libs.device.platform import Platform
from waflib import Options

class AdbProtocol(Protocol):
    name = 'adb'

    class Connection:
        def __init__(self, device_name):
            if Options.options.android_sdk_path:
                self.adb = os.path.join(Options.options.android_sdk_path, 'platform-tools', 'adb')
            else:
                self.adb = 'adb'
            self.device_name = device_name

        def __enter__(self):
            p = subprocess.Popen([self.adb, 'connect', self.device_name],
                                 stdin=subprocess.PIPE,
                                 stdout=subprocess.PIPE,
                                 stderr=subprocess.PIPE)
            out, err = p.communicate()
            p = subprocess.Popen([self.adb, 'devices'],
                                 stdin=subprocess.PIPE,
                                 stdout=subprocess.PIPE,
                                 stderr=subprocess.PIPE)
            out, err = p.communicate()
            if p.returncode:
                raise Exception('error connecting to device %s' % (self.device_name))
            if not isinstance(out, str):
                out = out.decode(sys.stdout.encoding, errors='ignore')
            for line in out.split('\n')[1:]:
                if line:
                    device_connection, status = line.split()
                    try:
                        device_name, port = device_connection.split(':')
                    except ValueError:
                        device_name = device_connection
                    if device_name == self.device_name:
                        if status != 'device':
                            raise Exception('device %s is in %s state' % (self.device_name, status))
                        break
            else:
                raise Exception('Could not connect to device %s' % (self.device_name))
            return self

        def __exit__(self, type, value, traceback):
            pass

        def _run(self, *command):
            try:
                p = subprocess.Popen((self.adb, '-s', self.device_name, 'shell') + command,
                                     stdin=subprocess.PIPE,
                                     stdout=subprocess.PIPE,
                                     stderr=subprocess.PIPE)
            except Exception as e:
                raise Exception('error running command "%s" on device %s' % (' '.join(command), self.device_name))
            else:
                out, err = p.communicate()
                if not isinstance(out, str):
                    out = out.decode(sys.stdout.encoding, errors='ignore')
                if not isinstance(err, str):
                    err = err.decode(sys.stderr.encoding, errors='ignore')
                if p.returncode:
                    raise Exception('command "%s" on device %s returned %d' % (' '.join(command), self.device_name, p.returncode))
                return (out, err)

        def status(self):
            out, err = self._run('getprop', 'ro.product.cpu.abi')
            out = out.strip()
            archs = [out]
            i = 1
            while True:
                i += 1
                out, err = self._run('getprop', 'ro.product.cpu.abi%d' % i)
                out = out.strip()
                if out:
                    archs.append(out)
                else:
                    break
            return ('online', 'Android', archs[0], archs)

        def mkdir(self, dir):
            pass

        def put(self, file, destination):
            pass

        def pull(self, file, destination):
            pass

        def start_shell(self):
            subprocess.Popen((self.adb, '-s', self.device_name, 'shell')).communicate()

        def install(self, package_file):
            subprocess.Popen((self.adb, '-s', self.device_name, 'install', '-r', package_file)).communicate()

        def start_activity(self, task_gen):
            pass

    def __init__(self, parameters):
        self.android_name = parameters

    def connect(self):
        return AdbProtocol.Connection(self.android_name)

    @classmethod
    def help(self, out):
        out.write('    Connects to an Android device through ADB.\n')
        out.write('    Syntax: %s://device_name\n' % self.name)


class AndroidPlatform(Platform):
    name = 'Android'
    names = ('android',)

    @classmethod
    def help(self, out):
        out.write('    supports Android devices through ADB\n')


def options(option_context):
    pass
