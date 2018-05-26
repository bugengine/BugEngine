from .scope import Scope

class Specifier:
    def __init__(self, specifier, position):
        self.specifier = specifier
        self.position = position

class ForStatement(Scope):
    def __init__(self, position):
        Scope.__init__(self, position)

    def set_init(self, init):
        self.init = init
