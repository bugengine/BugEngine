
class Constant:
    def __init__(self, type, name, value, position):
        self.type = type
        self.name = name
        self.value = value
        self.position = position

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def get_token_type(self):
        return 'VARIABLE_ID'
