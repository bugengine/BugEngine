
class Variable:
    def __init__(self, type, name, value, attributes, position):
        self.type = type
        self.name = name
        self.default_value = value
        self.attributes = attributes
        self.position = position

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def get_token_type(self):
        return 'VARIABLE_ID'

