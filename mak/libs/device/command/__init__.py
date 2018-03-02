import itertools

class Command(object):
    def __init__(self, devices):
        self.devices = devices

    def all_devices(self):
        return itertools.chain(*self.devices.values())

    @classmethod
    def bold_name(self):
        return '\x1b[01;37m%s\x1b[0m' % self.name

from .status import StatusCommand
from .list import ListCommand
from .add import AddCommand
from .remove import RemoveCommand
from .put import PutCommand
from .pull import PullCommand
from .shell import ShellCommand
