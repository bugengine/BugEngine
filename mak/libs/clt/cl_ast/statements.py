from .scope import Scope


class ForStatement(Scope):
    def __init__(self, parent, position):
        Scope.__init__(self, parent, position)

    def set_init(self, init):
        self.init = init
