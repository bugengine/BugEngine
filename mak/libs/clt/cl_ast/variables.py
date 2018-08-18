from .cppobject import CppObject


class Variable(CppObject):
    def __init__(self, parent, position, type, name, value, attributes):
        CppObject.__init__(self, parent, position)
        self.type = type
        self.name = name
        self.default_value = value
        self.attributes = attributes

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def get_token_type(self):
        return 'VARIABLE_ID'

    def write_to(self, writer):
        writer.create_variable(self.position, self.type.build_type(writer), self.name,
                               self.default_value and self.default_value.build_value(writer),
                               [writer.build_attribute(a[0], a[1]) for a in self.attributes])

    def _instantiate(self, parent, template_arguments):
        return Variable(parent,
                        self.position,
                        self.type.instantiate(parent, template_arguments),
                        self.name,
                        self.default_value and self.default_value.instantiate(parent,
                                                                              template_arguments),
                        self.attributes)

    def _debug_dump(self, indent):
        default_value = self.default_value and (' = %s' % self.default_value.to_string()) or ''
        print(indent + '%s %s%s;' % (self.type.type_name(), self.name, default_value))
