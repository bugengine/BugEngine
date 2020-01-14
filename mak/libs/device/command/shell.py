from . import Command


class ShellCommand(Command):
    name = 'shell'

    def run(self, device):
        for d in self.all_devices():
            if d.name == device:
                try:
                    with d.connect() as connection:
                        connection.start_shell()
                except Exception as e:
                    print(e)
                break
        else:
            raise Exception('Unknown device %s' % device)

    @classmethod
    def help(self, out):
        out.write('    %s <device_name>\n' % self.bold_name())
        out.write('        Starts a shell on device\n')
