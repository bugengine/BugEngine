from .cppobject import CppObject
from .scope import Scope
from .templates import Template, DependentName


class ConversionError(Exception):
    def __init__(self, message, position):
        self.message = message
        self.position = position


class Typedef(CppObject):
    def __init__(self, parent, position, name, type):
        CppObject.__init__(self, parent, position)
        self.name = name
        self.type = type

    def get_token_type(self):
        return 'TYPENAME_ID'

    def find_nonrecursive(self, name):
        if self.name == name:
            return self.type

    def type_name(self):
        return self.name

    def to_string(self):
        return self.name

    def find(self, name, is_current_scope):
        return self.type.find(name)

    #TODO: delete
    def original_type(self):
        return self.type

    def _instantiate(self, parent, template_arguments):
        return Typedef(parent, self.position, self.name,
                       self.type.instantiate(parent, template_arguments),)

    def signature(self):
        return self.type.signature()

    def _debug_dump(self, indent):
        print(indent + '/* typedef %s %s */' % (self.type.to_string(), self.name))

    def write_to(self, writer):
        pass


class DependentTypeName(DependentName):
    def __init__(self, parent, position, name):
        DependentName.__init__(self, parent, position, name)

    def get_token_type(self):
        return 'TYPENAME_ID'

    def type_name(self):
        result = []
        for name, params in zip(self.name.name, self.name.targets):
            if params[1]:
                result.append('%s<%s>' % (name, ', '.join([p.to_string() for p in params[1]])))
            else:
                result.append(name)
        return '::'.join(result)

    def to_string(self):
        return self.type_name()

    def original_type(self):
        return self.resolved_to

    def _instantiate(self, parent, template_arguments):
        result = DependentName._instantiate(self, parent, template_arguments)
        # TODO error check
        return result

    def try_conversion(self, other, qualifier_importance = False):
        if not isinstance(other, DependentTypeName):
            raise ConversionError("can't convert from %s type to %s type" % (self.__class__.__name__.lower(),
                                                                             other.__class__.__name__.lower()),
                                  self.position)
        if self.resolved_to:
            if self.resolved_to != other.resolved_to:
                print("can't convert from %s to %s" % ('::'.join(self.name.name),
                                                       '::'.join(other.name.name)))
                print(self.resolved_to, other.resolved_to)
                raise ConversionError("can't convert from %s to %s" % ('::'.join(self.name.name),
                                                                       '::'.join(other.name.name)),
                                      self.position)
        else:
            print('TODO')
            raise ConversionError("can't convert from %s to %s" % ('::'.join(self.name.name),
                                                                   '::'.join(other.name.name)),
                                  self.position)

    def signature(self):
        return 'typename{%s}' % self.type_name()

    def build_type(self, type_builder):
        print(self.name.name, self.position)
        assert(False)


class Struct(CppObject):
    id = 1
    class Definition(Scope):
        def __init__(self, parent, position, parent_struct):
            Scope.__init__(self, parent, position)
            self.parent_struct = parent_struct
            self.constructor = None
            self.destructor = None

    def __init__(self, parent, position, struct_type, name):
        CppObject.__init__(self, parent, position)
        self.id = Struct.id
        Struct.id += 1
        self.struct_type = struct_type
        self.name = name
        self.definition = None
        self.queued_templates = []

    def add(self, member):
        self.definition.add(member)

    def find(self, name, is_current_scope):
        if self.definition:
            if is_current_scope and name == self.name and self.definition.constructor:
                return self.definition.constructor
            else:
                return self.definition.find(name, is_current_scope)

    def define(self, parent):
        self.definition = Struct.Definition(self, self.position, parent)
        for instance, parameters in self.queued_templates:
            self._instantiate_content(instance, parameters)

    def get_token_type(self):
        return 'STRUCT_ID'

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def type_name(self):
        return self.name

    def to_string(self):
        return self.type_name()

    def signature(self):
        return '%s{%d}' % (self.name, self.id)

    def write_to(self, writer):
        if self.definition:
            with writer.create_struct(self.position, self.id, self.name,
                                      self.definition.parent_struct and self.definition.parent_struct.id) as s:
                if self.definition.constructor:
                    self.definition.constructor.write_to(s)
                if self.definition.destructor:
                    self.definition.destructor.write_to(s)
                for member in self.definition.members:
                    member.write_to(s)

    def build_type(self, writer):
        return writer.struct(self.name, self.id, self.position)

    def _instantiate(self, parent, template_arguments):
        return Struct(parent, self.position, self.struct_type, self.name)

    def _instantiate_content(self, instance, template_arguments):
        definition = self.definition
        if definition:
            instance.define(definition.parent_struct
                        and definition.parent_struct.instantiate(instance, template_arguments))
            for member in definition.members:
                print(member)
                instance.definition.members.append(member.instantiate(instance, template_arguments))
            if definition.constructor:
                instance.definition.constructor = definition.constructor.instantiate(instance, template_arguments)
            if definition.destructor:
                instance.definition.destructor = definition.destructor.instantiate(instance, template_arguments)
        else:
            self.queued_templates.append((instance, template_arguments))
        return instance

    def try_conversion(self, other, qualifier_importance = False):
        if not isinstance(other, Struct):
            raise ConversionError("can't convert from %s type to %s type" % (self.__class__.__name__.lower(),
                                                                             other.__class__.__name__.lower()),
                                  self.position)
        if not self.definition:
            raise ConversionError("struct %s is not defined" % (self.name),
                                  self.position)
        s = self
        while s:
            if s == other:
                return
            s = s.definition.parent_struct
        raise ConversionError("can't convert from %s to %s" % (self.name, other.name),
                              self.position)

    def _debug_dump(self, indent):
        if self.definition:
            parent_name = self.definition.parent_struct and (' : '+self.definition.parent_struct.type_name()) or ''
            print(indent + 'struct %s%s\n%s{' % (self.name, parent_name, indent))
            self.definition._debug_dump(indent + '  ')
            print(indent + '};\n%s' % indent)
        else:
            print(indent + 'struct %s;' % self.name)


class Builtin(CppObject):
    def __init__(self, parent, position, typename):
        CppObject.__init__(self, parent, position)
        self.typename = typename

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

    def build_type(self, writer):
        return writer.builtin(self.typename, self.position)

    def instantiate(self, parent, template_arguments):
        return self

    def to_string(self):
        return self.type_name()


class Type(CppObject):
    def __init__(self, parent, position, base_type):
        CppObject.__init__(self, parent, position)
        self.base_type = base_type
        self.modifier = []

    def find(self, name, is_current_scope):
        return self.base_type.find(name, is_current_scope)

    def add_modifier(self, modifier, position):
        self.modifier.append((modifier, position))

    def type_name(self):
        return ' '.join([self.base_type.type_name()] + [m[0] for m in self.modifier])

    def original_type(self):
        return self.base_type.original_type()

    def is_valid(self, type):
        from . import templates
        return isinstance(type, templates.TemplateParameterTypename)

    def signature(self):
        return self.base_type.signature()

    def try_conversion(self, other_type, qualifier_importance=False):
        self.base_type.try_conversion(other_type.base_type, True)
        if qualifier_importance:
            for modifier, position in self.modifier:
                for other_modifier, other_position in other_type.modifier:
                    if modifier == other_modifier: break
                else:
                    raise ConversionError("conversion would lose '%s' qualifier" % modifier,
                                          position)

    def build_type(self, writer):
        t = self.base_type.build_type(writer)
        for modifier, position in self.modifier:
            t.add_modifier(modifier, position)
        return t

    def _instantiate(self, parent, template_arguments):
        t = Type(parent, self.position, self.base_type.instantiate(parent, template_arguments))
        for modifier, position in self.modifier:
            t.add_modifier(modifier, position)
        return t

    def matches(self, other):
        try:
            self.try_conversion(other, True)
        except ConversionError:
            return False
        else:
            return True

    def to_string(self):
        return self.type_name()


class Pointer(CppObject):
    def __init__(self, parent, position, pointer_to):
        CppObject.__init__(self, parent, position)
        self.pointer_to = pointer_to

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

    def build_type(self, writer):
        return writer.pointer(self.pointer_to.build_type(writer), self.position)

    def _instantiate(self, parent, template_arguments):
        return Pointer(parent, self.position,
                       self.pointer_to.instantiate(parent, template_arguments))

    def to_string(self):
        return self.type_name()


class Reference(CppObject):
    def __init__(self, parent, position, pointer_to):
        CppObject.__init__(self, parent, position)
        self.pointer_to = pointer_to

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

    def build_type(self, writer):
        return writer.reference(self.pointer_to.build_type(writer), self.position)

    def _instantiate(self, parent, template_arguments):
        return Reference(parent, self.position,
                         self.pointer_to.instantiate(parent, template_arguments))

    def to_string(self):
        return self.type_name()


class Array(CppObject):
    def __init__(self, parent, position, array_type, count):
        CppObject.__init__(self, parent, position)
        self.array_type = array_type
        self.count = count

    def type_name(self):
        if self.count:
            return '%s[%s]' % (self.array_type.type_name(), self.count.to_string())
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

    def build_type(self, writer):
        return writer.array(self.array_type.build_type(writer), self.count, self.position)

    def _instantiate(self, parent, template_arguments):
        return Array(parent, self.position,
                     self.array_type.instantiate(parent, template_arguments),
                     self.count and self.count.instantiate(parent, template_arguments))

    def to_string(self):
        return self.type_name()
