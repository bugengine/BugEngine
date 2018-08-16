class ConversionError(Exception):
    def __init__(self, message, position):
        self.message = message
        self.position = position


class Typedef:
    def __init__(self, name, type, position):
        self.name = name
        self.type = type
        self.position = position

    def get_token_type(self):
        return 'TYPENAME_ID'

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def type_name(self):
        return self.name

    def find(self, name, is_current_scope):
        return self.type.find(name)

    #TODO: delete
    def original_type(self):
        return self

    def instantiate(self, template_arguments):
        return Typedef(self.name, self.type.instantiate(params), self.position)

    def signature(self):
        return self.type.signature()


class Typename:
    def __init__(self, name, default_value):
        self.name = name
        self.default_value = default_value

    def get_token_type(self):
        return 'TYPENAME_ID'

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def find(self, name, is_current_scope):
        return None

    def type_name(self):
        return self.name

    #TODO: delete
    def original_type(self):
        return self

    def instantiate(self, template_arguments):
        return template_arguments.get(self.name, self)

    def signature(self):
        return 'typename{%s}' % (self.name)


class DependentTypeName:
    def __init__(self, name):
        self.name = name
        self.resolved_to = None

    def get_token_type(self):
        return 'TYPENAME_ID'

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def find(self, name, is_current_scope):
        return None

    def type_name(self):
        return '::'.join(self.name.name)

    def original_type(self):
        return self.resolved_to

    def instantiate(self, template_arguments):
        return template_arguments.get(self.name, self)

    def signature(self):
        return 'typename{%s}' % self.type_name()


class Struct:
    id = 1
    class Definition:
        def __init__(self, parent):
            self.parent = parent
            self.members = []
            self.constructor = None
            self.destructor = None

    def __init__(self, struct_type, name, position):
        self.id = Struct.id
        Struct.id += 1
        self.name = name
        self.position = position
        self.struct_type = struct_type
        self.definition = None

    def add(self, member):
        self.definition.members.append(member)

    def find(self, name, is_current_scope):
        if self.definition:
            if is_current_scope and name == self.name and self.definition.constructor:
                return self.definition.constructor
            for m in self.definition.members:
                sub = m.find_nonrecursive(name)
                if sub:
                    return sub

    def define(self, parent):
        self.definition = Struct.Definition(parent)

    def instantiate(self, template_arguments):
        instance = Struct(self.struct_type, self.name, self.position)
        d = self.definition
        if d:
            instance.definition = Struct.Definition(d.parent)
            for m in d.members:
                instance.definition.members.append(m.instantiate(template_arguments))
            if d.constructor:
                instance.definition.constructor = d.constructor.instantiate(template_arguments)
            if d.destructor:
                instance.definition.destructor = d.destructor.instantiate(template_arguments)
        return instance

    def get_token_type(self):
        return 'STRUCT_ID'

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def type_name(self):
        return self.name

    def signature(self):
        return '%s{%d}' % (self.name, self.id)

    def write_to(self, writer):
        if self.definition:
            with writer.create_struct(self.position, self.id, self.name,
                                      self.definition.parent and self.definition.parent.id) as s:
                if self.definition.constructor:
                    self.definition.constructor.write_to(s)
                if self.definition.destructor:
                    self.definition.destructor.write_to(s)
                for member in self.definition.members:
                    member.write_to(s)


class Builtin:
    def __init__(self, typename, position):
        self.typename = typename
        self.position = position

    def type_name(self):
        return self.typename

    def original_type(self):
        return self

    def signature(self):
        return 'builtin{%s}' % self.typename

    def try_conversion(self, other, qualifier_importance=False):
        if not isinstance(other, Builtin):
            raise ConversionError("can't convert from %s type %s to %s type" % (self.__class__.__name__.lower(),
                                                                                self.typename,
                                                                                other.__class__.__name__.lower()),
                                  self.position)
        #TODO: actual conversions?


class Type:
    def __init__(self, base_type, position):
        self.base_type = base_type
        self.modifier = []
        self.position = position

    def add_modifier(self, modifier, position):
        self.modifier.append((modifier, position))

    def type_name(self):
        return ' '.join([self.base_type.type_name()] + [m[0] for m in self.modifier])

    def original_type(self):
        return self.base_type.original_type()

    def is_valid(self, type):
        return isinstance(type, Typename)

    def signature(self):
        return self.base_type.signature()

    def try_conversion(self, other_type, qualifier_importance=False):
        self.base_type.try_conversion(other_type.base_type, True)
        if qualifier_importance:
            for modifier, position in self.modifier:
                for other_modifier, other_position in other.modifier:
                    if modifier == other.modifier: break
                else:
                    raise ConversionError("conversion would lose '%s' qualifier" % modifier,
                                          position)


class Pointer:
    def __init__(self, pointer_to, position):
        self.pointer_to = pointer_to
        self.position = position

    def type_name(self):
        return self.pointer_to.type_name() + '*'

    def original_type(self):
        return self.pointer_to.original_type()

    def signature(self):
        return '%s*' % (self.pointer_to.signature())

    def try_conversion(self, other, qualifier_importance=False):
        if not isinstance(other, Pointer):
            raise ConversionError("can't convert from %s type to %s type" % (self.__class__.__name__.lower(),
                                                                             other.__class__.__name__.lower()),
                                  self.position)
        self.pointer_to.try_conversion(other.pointer_to, True)


class Reference:
    def __init__(self, pointer_to, position):
        self.pointer_to = pointer_to
        self.position = position

    def type_name(self):
        return self.pointer_to.type_name() + '&'

    def original_type(self):
        return self.pointer_to.original_type()

    def signature(self):
        return '%s&' % (self.pointer_to.signature())

    def try_conversion(self, other, qualifier_importance=False):
        if not isinstance(other, Reference):
            raise ConversionError("can't convert from %s type to %s type" % (self.__class__.__name__.lower(),
                                                                             other.__class__.__name__.lower()),
                                  self.position)
        self.pointer_to.try_conversion(other.pointer_to, True)


class Array:
    def __init__(self, array_type, count, position):
        self.array_type = array_type
        self.count = count
        self.position = position

    def type_name(self):
        if self.count:
            return '%s[%s]' % (self.array_type.type_name(), self.count.write_expr())
        else:
            return '%s[]' % (self.array_type.type_name())

    def signature(self):
        return '%s[%s]' % (self.array_type.signature(), count or '')

    def original_type(self):
        return self.array_type.original_type()

    def try_conversion(self, other, qualifier_importance=False):
        if not isinstance(other, Array):
            raise ConversionError("can't convert from %s type to %s type" % (self.__class__.__name__.lower(),
                                                                             other.__class__.__name__.lower()),
                                  self.position)
        self.pointer_to.try_conversion(other.pointer_to, True)
