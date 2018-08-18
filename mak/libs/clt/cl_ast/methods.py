from .cppobject import CppObject
from .scope import Scope
from .types import Type, Pointer, Struct
from .expressions import Specifier
from .templates import Template


class Parameter(CppObject):
    def __init__(self, parent, position, type, name, value, attributes):
        CppObject.__init__(self, parent, position)
        self.type = type
        self.name = name
        self.value = value
        self.attributes = attributes

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def get_token_type(self):
        return 'VARIABLE_ID'

    def _instantiate(self, parent, template_arguments):
        return Parameter(parent,
                         self.position,
                         self.type.instantiate(parent, template_arguments),
                         self.name,
                         self.value and self.value.instantiate(parent, template_arguments),
                         self.attributes)


class Overload(Scope):
    def __init__(self, parent, position, owner, return_type, parameters, attributes, is_member):
        Scope.__init__(self, parent, position)
        self.owner = owner
        self.return_type = return_type
        self.parameters = parameters
        self.is_member = is_member
        self.this = 0
        if self.is_member:
            if 'static' not in [a.specifier for a in attributes]:
                rtype = Type(self, is_member.position, is_member)
                for a in attributes:
                    if a.specifier == 'const':
                        rtype.add_modifier('const', a.position)
                rtype = Type(self, is_member.position, Pointer(self, is_member.position, rtype))
                p = Parameter(self, is_member.position, rtype, 'this', None, [])
                self.parameters = [p] + self.parameters
                self.this = 1
        for p in self.parameters:
            self.add(p)
        self.attributes = attributes
        self.definition = None
        self.signature = '|'.join((p.type.signature() for p in self.parameters))
        self.queued_templates = []

    def define(self, definition):
        self.definition = definition
        for instance, parameters in self.queued_templates:
            instance.define(self.definition.instantiate(instance, template_arguments))

    def _instantiate(self, parent, template_arguments, owner):
        result = Overload(parent, self.position, owner,
                          self.return_type and self.return_type.instantiate(parent, template_arguments),
                          [p.instantiate(self, template_arguments) for p in self.parameters[self.this:]],
                          self.attributes,
                          self.is_member)
        for member in self.members:
            result.add(member.instantiate(self, template_arguments))
        if self.definition:
            result.define(self.definition.instantiate(result, template_arguments))
        else:
            self.queued_templates.append((result, template_arguments))
        return self

    def _debug_dump(self, indent):
        parameters = ', '.join(['%s %s' % (p.type.type_name(), p.name) for p in self.parameters])
        attributes = ' '.join([a.specifier for a in self.attributes])
        if self.definition:
            print(indent + '%s %s(%s) %s\n%s{' % (self.return_type and self.return_type.type_name() or 'void',
                                                  self.owner.name, parameters, attributes, indent))
            self.definition._debug_dump(indent + '  ')
            print(indent + '}\n%s' % indent)
        else:
            print(indent + '%s %s(%s) %s;' % (self.return_type and self.return_type.type_name() or 'void',
                                              self.owner.name, parameters, attributes))


class Method(CppObject):
    id = 1
    def __init__(self, parent, position, method_name):
        CppObject.__init__(self, parent, position)
        self.id = Method.id
        Method.id += 1
        self.name = method_name
        self.overloads = []
        while isinstance(parent, Template):
            parent = parent.parent
        if isinstance(parent, Struct):
            self.owner = parent
        else:
            self.owner = None

    def get_token_type(self):
        return 'METHOD_ID'

    def find(self, name, is_current_scope):
        return None

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def find_overload(self, position, return_type, parameters, attributes, create_if_necessary):
        new_overload = Overload(self, position, self, return_type, parameters,
                                attributes, self.owner)
        new_overload_static = Overload(self, position, self, return_type, parameters,
                                       attributes+[Specifier(self.parent, position, 'static')],
                                       self.owner)
        for overload in self.overloads:
            if overload.signature == new_overload.signature:
                return overload
            if not create_if_necessary and overload.signature == new_overload_static.signature:
                return overload
        if create_if_necessary:
            self.overloads.append(new_overload)
            return new_overload
        else:
            return None

    def write_to(self, writer):
        for overload_id, overload in enumerate(self.overloads):
            with writer.create_method(self.position, self.id, self.name, overload_id) as m:
                pass

    def _instantiate(self, parent, template_arguments):
        return Method(parent, self.position, self.name)

    def _instantiate_content(self, instance, template_arguments):
        for overload in self.overloads:
            instance.overloads.append(overload.instantiate(instance, template_arguments, self.owner))
        return instance

    def _debug_dump(self, indent):
        for overload in self.overloads:
            overload._debug_dump(indent)


class SpecialMethod(Method):
    def get_token_type(self):
        return 'SPECIAL_METHOD_ID'

class Body(Scope):
    def _instantiate(self, parent, template_arguments):
        result = Body(parent, self.position)
        for m in self.members:
            result.add(m.instantiate(result, template_arguments))
        return result
