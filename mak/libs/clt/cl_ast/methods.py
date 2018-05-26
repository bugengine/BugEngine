from .scope import Scope

class Method(Scope):
    def __init__(self, method_name, return_type, attributes, position):
        Scope.__init__(self, position)
        self.name = method_name
        self.return_type = return_type
        self.attributes = attributes
        self.definition = None

    def define(self, definition):
        self.definition = definition

    def get_token_type(self):
        return 'METHOD_ID'

    def find_nonrecursive(self, name):
        if self.name == name:
            return self
