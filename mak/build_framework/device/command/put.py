import os
from . import Command


class PutCommand(Command):
    name = 'put'

    def _copy_directory(self, connection, dir, destination):
        for root, dirs, files in os.walk(dir):
            relpath = os.path.relpath(root, dir)
            for d in dirs:
                try:
                    connection.mkdir(os.path.join(destination, relpath, d))
                except OSError as e:
                    pass
            for f in files:
                connection.put(os.path.join(root, f), os.path.join(destination, relpath, f))

    def _copy_file(self, connection, file, destination):
        connection.put(file, destination)

    def do_copy(self, device, destination, files):
        with device.connect() as connection:
            if len(files) == 1:
                target_folder = os.path.dirname(destination)
            else:
                target_folder = destination
            if target_folder:
                try:
                    connection.mkdir(target_folder)
                except OSError as e:
                    pass
            for file in files:
                if os.path.isdir(files[0]):
                    self._copy_directory(connection, files[0], destination)
                else:
                    self._copy_file(connection, files[0], destination)

    def run(self, device, arg1, arg2, *kw):
        args = (
            arg1,
            arg2,
        ) + kw
        for d in self.all_devices():
            if d.name == device:
                try:
                    self.do_copy(d, args[-1], args[:-1])
                except Exception as e:
                    print(e)
                break
        else:
            raise Exception('Unknown device %s' % device)

    @classmethod
    def help(self, out):
        out.write('    %s <device> <file1> [<file2>...] <destination>\n' % self.bold_name())
        out.write('        Uploads files or directories to the device\n')
