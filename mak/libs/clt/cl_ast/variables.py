from .cppobject import CppObject


class Variable(CppObject):
    def __init__(self, lexer, position, name, type, value, attributes):
        CppObject.__init__(self, lexer, position, name)
        self.type = type
        self.value = value
        self.attributes = attributes

    def get_token_type(self):
        return 'VARIABLE_ID'
        
    def debug_dump(self, indent=''):
        attrs = self.attributes and (' '.join(str(a) for a in self.attributes) + ' ') or ''
        value = self.value and ' = %s'%str(self.value) or ''
        print('%s%s%s%s%s [%s]' % (indent, attrs, str(self.type),
                                   self.name and (' %s'%self.name) or '',
                                   value, self.position))

    def _create_template_instance(self, template, arguments, position):
        return Variable(self.lexer, self.position, self.name,
                        self.type.create_template_instance(template, arguments, position),
                        self.value and self.value.create_template_instance(template, arguments, position),
                        self.attributes)
