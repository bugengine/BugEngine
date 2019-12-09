from .cppobject import CppObject
from .scope import Scope, ScopeError
from . import types


class Parameter(CppObject):
    def __init__(self, lexer, position, name, type, default_value):
        CppObject.__init__(self, lexer, position, name)
        self.type = type
        self.default_value = default_value

    def get_token_type(self):
        return 'VARIABLE_ID'

    def _create_template_instance(self, template, arguments, position):
        return Parameter(self.lexer, self.position, self.name,
                         self.type.create_template_instance(template, arguments, position),
                         self.default_value and self.default_value.create_template_instance(template, arguments, position))


class CodeBlock(Scope):
    def is_definition_scope(self):
        return False


class OverloadScope(CodeBlock):
    def find(self, name, position, source_context, is_current_scope):
        try:
            result = CodeBlock.find(self, name, position, source_context, is_current_scope)
        except ScopeError:
            result = None
        if not result:
            for v in self.owner.parameters:
                if v.name == name:
                    return v
            if not result and is_current_scope:
                raise ScopeError("no member named '%s' in %s" % (name, self.owner.pretty_name()), position)
        return result


class Overload(CppObject):
    def __init__(self, lexer, position, name, template, parameters, return_type, attributes):
        CppObject.__init__(self, lexer, position, name)
        self.parameters = parameters[:]
        self.return_type = return_type
        self.template = template
        if return_type:
            assert not return_type.is_void()
        self.attributes = attributes

    def match(self, parameters, position, return_type, attributes, binding):
        #attributes should be reflected in argument signature instread
        if 'const' in self.attributes and not 'const' in attributes:
            return False
        if not 'const' in self.attributes and 'const' in attributes:
            return False
        if len(parameters) != len(self.parameters):
            return False
        for p1, p2 in zip(parameters, self.parameters):
            try:
                d = p1.type.distance(p2.type, types.CastOptions(types.CastOptions.CAST_NONE, template_bindings=binding))
            except types.CastError:
                return False
            else:
                if not d.exact_match():
                    return False
        for p1, p2 in zip(self.parameters, parameters):
            if p1.default_value and p2.default_value:
                self.lexer.error('redefinition of default argument', p2.position)
                self.lexer.note('previous definition is here', p1.position)
        if self.return_type and return_type:
            try:
                return_type.distance(self.return_type, types.CastOptions(types.CastOptions.CAST_NONE, template_bindings=binding))
            except types.CastError:
                self.lexer.error('functions that differ only by return type cannot be overloaded', return_type.position)
                self.lexer.note('previous declaration is here', self.return_type.position)
        elif return_type:
            if not return_type.is_void():
                self.lexer.error('functions that differ only by return type cannot be overloaded', return_type.position)
                self.lexer.note('previous declaration is here', self.position)
        elif self.return_type:
            self.lexer.error('functions that differ only by return type cannot be overloaded', position)
            self.lexer.note('previous declaration is here', self.return_type.position)
        return True

    def _create_template_instance(self, template, arguments, position):
        return Overload(self.lexer, self.position, self.name,
                        self.template and  self.template.create_template_instance(template, arguments, position),
                        [p.create_template_instance(template, arguments, position) for p in self.parameters],
                        self.return_type and self.return_type.create_template_instance(template, arguments, position),
                        self.attributes)

    def debug_dump(self, indent):
        print('%s%s%s(%s);\n' % (indent, self.return_type and ('%s '%self.return_type) or '', self.name,
                                 ', '.join('%s%s'%(str(p.type), p.name and (' %s'%p.name) or '') for p in self.parameters)))

    def debug_dump_overload(self, indent):
        if self.template:
            self.template.debug_dump(indent)
        else:
            self.debug_dump(indent)


class Method(CppObject):
    index = 1
    def __init__(self, lexer, position, name, owner):
        CppObject.__init__(self, lexer, position, name)
        self.id = Method.index
        self.owner = owner
        Method.index += 1
        self.overloads = []

    def find_overload(self, template, parameters, position, return_type, attributes):
        for o in self.overloads:
            if bool(template) == bool(o.template):
                binding = {}
                if template:
                    if len(template.parameters) != len(o.template.parameters):
                        continue
                    binding = template.fill_temporary_binding(o.template, binding)
                if o.match(parameters, position, return_type, attributes, binding):
                    return o
        return None

    def create_overload(self, template, parameters, position, return_type, attributes):
        if return_type and return_type.is_void():
            return_type = None
        o = Overload(self.lexer, position, self.name, template, parameters, return_type, attributes)
        self.overloads.append(o)
        if template:
            template.scope.add(o)
        return o

    def get_token_type(self):
        return 'METHOD_ID'

    def _create_template_instance(self, template, arguments, position):
        return Method(self.lexer, self.position, self.name, self.owner.create_template_instance(template, arguments, position))

    def _complete_template_instance(self, result, template, arguments, position):
        for o in self.overloads:
            result.overloads.append(o.create_template_instance(template, arguments, position))
        return result

    def write_to(self, writer):
        for i, o in enumerate(self.overloads):
            with writer.create_method(o.position, self.id, self.name, i) as method:
                pass

    def debug_dump(self, indent):
        for overload in self.overloads:
            overload.debug_dump_overload(indent)


class SpecialMethod(Method):
    def __init__(self, lexer, position, name, owner):
        Method.__init__(self, lexer, position, name, owner)

    def get_token_type(self):
        return 'SPECIAL_METHOD_ID'
