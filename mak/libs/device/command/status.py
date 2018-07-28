from . import Command

class StatusCommand(Command):
    name = 'status'
    colors = {
        'online':   u'\x1b[01;32m\u25cf\x1b[0m',
        'standby':  u'\x1b[01;33m\u25cf\x1b[0m',
        'offline':  u'\x1b[37m\u25cf\x1b[0m',
        'error':    u'\x1b[01;31m\u25cf\x1b[0m',
        'unknown':  u'\x1b[01;35m?\x1b[0m'
    }
    def get_device(self, name):
        for d in self.all_devices():
            if d.name == name:
                return d

    def run(self, *kw):
        if kw:
            devices = [(n, self.get_device(n)) for n in kw]
        else:
            devices = [(d.name, d) for d in self.all_devices()]
        device_name_length = max([len(name) for name, device in devices])
        for name, device in devices:
            if device:
                status = device.get_status()
                if status.connection_status in ('online', 'standby'):
                    print('%s %s%s[%s - %s]' % (self.colors[status.connection_status],
                                               name,
                                               ' '*(device_name_length + 2 - len(name)),
                                               status.platform,
                                               status.arch))
                else:
                    print('%s %s%s%s' % (self.colors[status.connection_status],
                                         name,
                                         ' '*(device_name_length + 2 - len(name)),
                                         status.message))
            else:
                print('%s %s%s[unknown device]' % (self.colors['uknown'],
                                                   name,
                                                   ' '*(device_name_length + 2 - len(name))))

    @classmethod
    def help(self, out):
        out.write('    %s [<device_name>...]\n' % self.bold_name())
        out.write('        Shows the status of the specified devices, or all if no device is specified\n')

