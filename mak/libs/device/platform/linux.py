from . import Platform

class Linux(Platform):
    name = 'linux'

    def __init__(self):
        pass

    @classmethod
    def help(self, out):
        out.write('    supports Linux as a target\n')
