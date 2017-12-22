import sys
from . import Command
from ..platform import Platform
from ..protocol import Protocol


class ListCommand(Command):
    name = 'list'
    categories = {
        'protocols': Protocol,
        'commands': Command,
        'platforms': Platform,
    }

    def __init__(self, devices):
        self.devices = devices

    def run(self, category):
        try:
            cls = self.categories[category]
        except KeyError:
            raise TypeError('unknown category %s' % category)
        else:
            print('registered %s:' % category)
            for subclass in cls.__subclasses__():
                print('\n\x1b[01;34m%s\x1b[0m' % subclass.name)
                subclass.help(sys.stdout)

    @classmethod
    def help(self, out):
        out.write('    %s %s\n' % (self.bold_name(), '|'.join(sorted(self.categories.keys()))))
        out.write('        Displays a lits of registered items of the specified category\n')

