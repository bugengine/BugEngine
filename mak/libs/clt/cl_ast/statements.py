from .scope import Scope


class ForStatement(Scope):
    def __init__(self, position):
        Scope.__init__(self, position)

    def set_init(self, init):
        self.init = init
