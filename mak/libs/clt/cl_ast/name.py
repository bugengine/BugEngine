from .cppobject import CppObject


class Name:
    def __init__(self, lexer, name, position, target=None, template=None, arguments=None,
                 template_bindings=None, parent=None, data=None, shadow=False, errors=[]):
        self.lexer = lexer
        self.name = name
        self.position = position
        self.target = target
        self.template = template
        self.arguments = arguments
        self.template_bindings = template_bindings or (parent and parent.template_bindings)
        self.parent = parent
        self.dependent = parent and parent.dependent or False
        self.errors = errors[:]
        self.shadow = shadow
        if shadow:
            assert not parent
        self.data = data

    def __str__(self):
        if self.arguments:
            name = '%s<%s>' % (self.name, (', '.join(str(a) for a in self.arguments)))
        else:
            name = self.name
        if self.parent:
            return str(self.parent) + '::' + name
        else:
            return name

    def equals(self, other, template_bindings):
        if not other:
            return False
        if self.dependent:
            if self.parent != other.parent:
                return False
            if self.name != other.name:
                return False
            if self.template != other.template:
                return False
            if self.template:
                args1 = self.template.expand_template_arg_list(self.arguments)
                args2 = self.template.expand_template_arg_list(other.arguments)
                if not self.template.equal_parameters(args1, args2, template_bindings):
                    return False
            else:
                if not CppObject.equal_parameters(self.arguments or [], other.arguments or [], template_bindings):
                    return False
            return True
        else:
            return self.target == other.target
        return False

    def __neq__(self, other):
        return not (self == other)

    def get_type(self):
        if self.target:
            return self.target.get_token_type()
        else:
            return 'ID'

    def is_dependent(self):
        return self.dependent

    def is_shadow(self):
        return self.shadow

    def is_qualified(self):
        return self.parent != None
    
    def bind(self):
        if self.template:
            if not self.template_bindings:
                #assert self.shadow, self
                pass
            else:
                self.template_bindings.template.bind(self.template)
        self.parent and self.parent.bind()
        return self
    
    def show_errors(self):
        self.parent and self.parent.show_errors()
        for error in self.errors:
            self.lexer.error(error, self.position)
