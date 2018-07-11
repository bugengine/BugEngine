from . import types


class Constant:
    def __init__(self, type, name, value, position):
        self.type = type
        self.name = name
        self.value = value
        self.position = position

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def instantiate(self, template_arguments):
        return self

    def get_token_type(self):
        return 'VARIABLE_ID'

    def is_valid(self, other):
        if not isinstance(other, Constant):
            raise types.ConversionError('cannot convert from %s to %s constant' % (other.__class__.__name__.lower(),
                                                                                   self.type.name()),
                                         self.position)
        self.type.try_conversion(other.type)


class Value:
    def __init__(self, type, value, position):
        self.type = type
        self.value = value
        self.position = position

    def find_nonrecursive(self, name):
        return None

    def is_valid(self, other):
        if not isinstance(other, Constant):
            raise types.ConversionError('cannot convert from %s to %s constant' % (other.__class__.__name__.lower(),
                                                                                   self.type.name()),
                                         self.position)
        self.type.try_conversion(other.type)


class DependentValueName:
    def __init__(self, name):
        self.name = name
        self.resolved_to = None

    def get_token_type(self):
        return 'VARIABLE_ID'

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def find(self, name, is_current_scope):
        return None

    def instantiate(self, template_arguments):
        return template_arguments.get(self.name, self)
