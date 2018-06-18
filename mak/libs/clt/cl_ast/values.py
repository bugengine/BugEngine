
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


class Value:
    def __init__(self, type, value, position):
        self.type = type
        self.value = value
        self.position = position

    def find_nonrecursive(self, name):
        return None

    def is_valid(self, type):
        return False


class DependentValueName:
    def __init__(self, name):
        self.name = name
        self.resolved_to = None

    def get_token_type(self):
        return 'VARIABLE_ID'

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def find(self, name):
        return None

    def instantiate(self, template_arguments):
        return template_arguments.get(self.name, self)
