import os
import shutil
import subprocess
from . import Protocol


class LocalhostProtocol(Protocol):
    name = 'localhost'

    class Connection:
        def __init__(self):
            pass

        def __enter__(self):
            return self

        def __exit__(self, type, value, traceback):
            pass

        def status(self):
            import platform
            return ('online', platform.system(), platform.machine(), [platform.machine()])

        def mkdir(self, dir):
            os.makedirs(dir)

        def put(self, file, destination):
            shutil.copy(file, destination)

        def pull(self, file, destination):
            shutil.copy(file, destination)

        def start_shell(self):
            subprocess.Popen([os.environ['SHELL'], '-i', '-l']).communicate()

    def __init__(self, connection):
        pass

    def connect(self):
        return LocalhostProtocol.Connection()

    @classmethod
    def help(self, out):
        out.write('    Registers the local machine as a device\n')
