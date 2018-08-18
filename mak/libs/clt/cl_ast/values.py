from .cppobject import CppObject
from . import types
from . import templates


class Value(CppObject):
    def __init__(self, parent, position, type, value):
        CppObject.__init__(self, parent, position)
        self.type = type
        self.value = value

    def find_nonrecursive(self, name):
        return None

    def is_valid(self, other):
        if not isinstance(other, templates.TemplateParameterConstant):
            raise types.ConversionError('cannot convert from %s to %s constant' % (other.__class__.__name__.lower(),
                                                                                   self.type.name()),
                                         self.position)
        self.type.try_conversion(other.type)

    def to_string(self):
        return '(%s)%s' % (self.type.type_name(), str(self.value))

    def matches(self, other):
        return self.type.matches(other.type) and self.value == other.value

    def _instantiate(self, parent, template_arguments):
        return Value(parent, self.position,
                     self.type.instantiate(parent, template_arguments), self.value)


class DependentValueName(templates.DependentName):
    def __init__(self, parent, position, name):
        templates.DependentName.__init__(self, parent, position, name)

    def get_token_type(self):
        return 'VARIABLE_ID'

    def _instantiate(self, parent, template_arguments):
        result = emplates.DependentName._instantiate(self, parent, template_arguments)
        # TODO: type check
        return result

    def to_string(self):
        return '::'.join(self.name.name)
