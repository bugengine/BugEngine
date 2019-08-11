from .cppobject import CppObject
from .scope import Scope
from .name import Name


class CastError(Exception):
    pass


CAST_NONE=0
CAST_ATTRIB=1
CAST_IMPLICIT=2
CAST_STATIC=3
CAST_UPCAST=4
CAST_REINTERPRET=5
CAST_UNRELATED=6


class Type(CppObject):
    class Distance:
        def __init__(self, variant=0, cast=0, attribute=0, matches={}):
            self.distance = (variant, cast, attribute)
            self.matches = {}
            for a, m in matches.items():
                self.matches[a] = m

        def refine(self):
            if self.distance[0] > 0:
                self.distance = (self.distance[0]-1, self.distance[1], self.distance[2])
            return self

        def __bool__(self):
            return self.distance[0] >= 0 and self.distance[1] >= 0 and self.distance[2] >= 0
        
        def exact_match(self):
            return self.distance[0] == 0 and self.distance[1] == 0 and self.distance[2] == 0

        def match_attributes(self, allowed_cast, attrs1, attrs2):
            for a in attrs2:
                if a not in attrs1:
                    raise CastError()
            cast_cost = 0
            for a in attrs1:
                if a not in attrs2:
                    cast_cost += 1
            if not allowed_cast:
                if cast_cost:
                    raise CastError()
            self.distance = (self.distance[0], self.distance[1], self.distance[2]+cast_cost)
            return self

        def __eq__(self, other):
            return self.distance == other.distance

        def __ne__(self, other):
            return self.distance != other.distance

        def __lt__(self, other):
            return self.distance < other.distance

        def __le__(self, other):
            return self.distance <= other.distance

        def __gt__(self, other):
            return self.distance > other.distance

        def __ge__(self, other):
            return self.distance >= other.distance
        
        def __iadd__(self, other):
            for k, v in other.matches.items():
                if k in self.matches:
                    d = v.distance(self.matches[k], CAST_NONE)
                    if d != type.Distance():
                        raise CastError()
                else:
                    self.matches[k] = v
            self.distance = (self.distance[0] + other.distance[0],
                             self.distance[1] + other.distance[1],
                             self.distance[2] + other.distance[2],)
            return self
        
        def __repr__(self):
            return 'd[%s,%s,%s]' % self.distance

        def __str__(self):
            return 'variant=%d / attribute = %d / cast = %d' % (self.distance[0], self.distance[1], self.distance[2])

    def __init__(self, *args):
        CppObject.__init__(self, *args)

    def get_type(self):
        return self

    def get_unresolved_parameters(self):
        return []


class TypeDef(Type):
    class ErrorScope(Scope):
        def __init__(self, owner, position):
            Scope.__init__(self, owner, position)

        def find(self, name, position, is_current_scope):
            if is_current_scope:
                self.owner.lexer._error('f', position)
                self.owner.lexer._note('f',self.position)
            return None

    def __init__(self, lexer, position, name, type):
        Type.__init__(self, lexer, position, name)
        self.type = type
        assert isinstance(self.type, TypeRef)
        self.scope = getattr(self.type.type, 'scope', None)
        if not self.scope:
            self.scope = self.ErrorScope(self, self.position)

    def get_type(self):
        return self.type.get_type()

    def __str__(self):
        return str(self.type)

    def get_token_type(self):
        return 'TYPENAME_ID'


class Pointer(Type):
    def __init__(self, lexer, position, type):
        Type.__init__(self, lexer, position)
        self.type = type

    def __str__(self):
        return str(self.type)+'*'
    
    def _distance(self, other, matches, typeref, other_typeref, allowed_cast):
        if isinstance(other, Pointer):
            if allowed_cast in (CAST_STATIC, CAST_UPCAST):
                target_cast = CAST_UPCAST
            elif allowed_cast in (CAST_REINTERPRET, CAST_UNRELATED):
                target_cast = CAST_UNRELATED
            else:
                target_cast = allowed_cast
            d = self.type.distance(other.type, target_cast, matches).refine()
            return d.match_attributes(allowed_cast, typeref.qualifiers, other_typeref.qualifiers)
        elif allowed_cast == CAST_UNRELATED:
            return Type.Distance(variant=-1)
        else:
            raise CastError()

    def _create_template_instance(self, template, arguments, position):
        return Pointer(self.lexer, self.position,
                       self.type.create_template_instance(template, arguments, self.position))

    def signature(self):
        return 'P'+self.type.signature()


class Reference(Type):
    def __init__(self, lexer, position, type):
        Type.__init__(self, lexer, position)
        self.type = type

    def __str__(self):
        return str(self.type)+'&'

    def _distance(self, other, matches, typeref, other_typeref, allowed_cast):
        if isinstance(other, Reference):
            if allowed_cast in (CAST_STATIC, CAST_UPCAST):
                target_cast = CAST_UPCAST
            elif allowed_cast in (CAST_REINTERPRET, CAST_UNRELATED):
                target_cast = CAST_UNRELATED
            else:
                target_cast = allowed_cast
            d = self.type.distance(other.type, target_cast).refine()
            return d.match_attributes(allowed_cast, typeref.qualifiers, other_typeref.qualifiers)
        elif allowed_cast == CAST_UNRELATED:
            return Type.Distance(variant=-1)
        else:
            raise CastError()

    def _create_template_instance(self, template, arguments, position):
        return Reference(self.lexer, self.position,
                         self.type.create_template_instance(template, arguments, self.position))

    def signature(self):
        return 'R'+self.type.signature()


class Array(Type):
    def __init__(self, lexer, position, type, size):
        Type.__init__(self, lexer, position)
        self.type = type
        self.size = size

    def __str__(self):
        return '%s[%s]' % (str(self.type), self.size or '')

    def _distance(self, other, matches, typeref, other_typeref, allowed_cast):
        if isinstance(other, Array):
            if allowed_cast in (CAST_STATIC, CAST_UPCAST):
                target_cast = CAST_UPCAST
            elif allowed_cast in (CAST_REINTERPRET, CAST_UNRELATED):
                target_cast = CAST_UNRELATED
            else:
                target_cast = allowed_cast
            d = self.type.distance(other.type, target_cast).refine()
            return d.match_attributes(allowed_cast, typeref.qualifiers, other_typeref.qualifiers)
        elif allowed_cast == CAST_UNRELATED:
            return Type.Distance(variant=-1)
        else:
            raise CastError()

    def _create_template_instance(self, template, arguments, position):
        return Array(self.lexer, self.position,
                     self.type.create_template_instance(template, arguments, self.position),
                     self.size and self.size.create_template_instance(template, arguments, self.position))

    def signature(self):
        return 'P'+self.type.signature()


class StructScope(Scope):
    def __init__(self, owner, position, parent):
        Scope.__init__(self, owner, position)
        self.parent = parent
        self.constructor = None
        self.destructor = None
        self.casts = []
        self.operators = []

    def debug_dump(self, indent):
        if self.constructor:
            self.constructor.debug_dump(indent)
        if self.destructor:
            self.destructor.debug_dump(indent)
        for _, cast in self.casts:
            cast.debug_dump(indent)
        Scope.debug_dump(self, indent)

    def find(self, name, position, is_current_scope):
        if is_current_scope:
            if name == self.owner.name:
                if self.owner.lexer.last_token == '~':
                    return self.destructor
                else:
                    return self.constructor
        return Scope.find(self, name, position, is_current_scope)


class Struct(Type):
    global_index = 0

    def __init__(self, lexer, position, struct_type, name):
        Type.__init__(self, lexer, position, name)
        self.struct_type = struct_type
        self.index = Struct.global_index
        Struct.global_index += 1

    def get_token_type(self):
        return 'STRUCT_ID'

    def define(self, parent, position):
        self.push_scope(position, StructScope(self, position, parent))

    def _create_template_instance(self, template, arguments, position):
        return Struct(self.lexer, self.position, self.struct_type, self.name)

    def _complete_template_instance(self, result, template, arguments, position):
        if self.scope:
            result.define(self.scope.parent and self.scope.parent.create_template_instance(template, arguments, position).get_type(), position)
            try:
                self.scope.create_template_instance(result.scope, template, arguments, position)
            except Exception:
                self.lexer.pop_scope(result.scope)
                raise
            else:
                self.lexer.pop_scope(result.scope)
        return result

    def _distance(self, other, matches, typeref, other_typeref, allowed_cast):
        if isinstance(other, Struct):
            if allowed_cast <= CAST_ATTRIB:
                if other != self:
                    raise CastError()
                else:
                    d = Type.Distance()
                    return d.match_attributes(allowed_cast, typeref.qualifiers, other_typeref.qualifiers)
            cast = 0
            variant = 0
            p = self
            while p:
                if p == other:
                    break
                cast += 1
                p = p.scope and p.scope.parent
            else:
                cast = 0
                p = other
                while p:
                    if p == self:
                        break
                    cast -= 1
                else:
                    if allowed_cast < CAST_UNRELATED:
                        raise CastError()
                    variant = -1
                    cast = 0
            d = Type.Distance(variant=variant, cast=cast)
            return d.match_attributes(allowed_cast, typeref.qualifiers, other_typeref.qualifiers)
        elif allowed_cast == CAST_UNRELATED:
            return Type.Distance(variant = -1)
        else:
            raise CastError()

    def __str__(self):
        return self.name or '<anonymous>'

    def signature(self):
        return '%s_%d' % (self.name or 'anonymous', self.index)

    def write_to(self, writer):
        with writer.create_struct(self.position, self.index, self.name, self.scope.parent and self.scope.parent.index) as struct:
            if self.scope:
                self.scope.write_to(struct)


class BuiltIn(Type):
    def __init__(self, lexer, position, builtin):
        Type.__init__(self, lexer, position)
        self.builtin = builtin

    def __str__(self):
        return self.builtin

    def _distance(self, other, matches, typeref, other_typeref, allowed_cast):
        if isinstance(other, BuiltIn):
            if self.builtin != other.builtin:
                if allowed_cast >= CAST_IMPLICIT:
                    d = Type.Distance(cast=1)
                    return d.match_attributes(allowed_cast, typeref.qualifiers, other_typeref.qualifiers)
                else:
                    raise CastError()
            else:
                d = Type.Distance(cast=0)
                return d.match_attributes(allowed_cast, typeref.qualifiers, other_typeref.qualifiers)
        elif allowed_cast == CAST_UNRELATED:
            d = Type.Distance(variant = -1)
            return d.match_attributes(allowed_cast, typeref.qualifiers, other_typeref.qualifiers)
        else:
            raise CastError()

    def _create_template_instance(self, template, arguments, position):
        return self
    
    def signature(self):
        return '%d%s' % (len(self.builtin), self.builtin)


class Void(Type):
    def __init__(self, lexer, position):
        Type.__init__(self, lexer, position)

    def __str__(self):
        return 'void'

    def _distance(self, other, matches, typeref, other_typeref, allowed_cast):
        if isinstance(other, Void):
            d = Type.Distance(cast=0)
            return d.match_attributes(allowed_cast, typeref.qualifiers, other_typeref.qualifiers)
        elif allowed_cast == CAST_UNRELATED:
            d = Type.Distance(variant = -1)
            return d.match_attributes(allowed_cast, typeref.qualifiers, other_typeref.qualifiers)
        else:
            raise CastError()

    def _create_template_instance(self, template, arguments, position):
        return self

    def signature(self):
        return '4void'


class DependentTypeName(Type):
    def __init__(self, lexer, position, name):
        Type.__init__(self, lexer, position)
        self.name = name

    def get_unresolved_parameters(self):
        result = []
        for target, name_arguments, name_template in self.name.targets:
            if target:
                result += target.get_unresolved_parameters()
                for a in name_arguments:
                    result += a.get_unresolved_parameters()
                if name_template:
                    result += name_template.get_unresolved_parameters()
        return result

    def resolve(self, current_object, name, position, arguments, instance_template, instance_arguments, instance_position):
        from .templates import Template
        if not current_object:
            _, result = self.lexer.lookup_by_name(name, position)
            assert result
        else:
            result = current_object.scope.find(name, position, True)
        if not result:
            raise Template.InstantiationError(position, 'no object named %s in %s' % (name, current_object.name))
        result = result.create_template_instance(instance_template, instance_arguments, instance_position)
        if arguments:
            try:
                result = result.instantiate([a.create_template_instance(instance_template, instance_arguments, instance_position) for a in arguments], position)
            except Template.InstantiationError as e:
                raise Template.InstantiationError(position, "in instantiation of template '%s' requested here"%name, e)
        return result

    def _create_partial_template_instance(self, template, arguments, position):
        result = None
        for name, pos, (_, name_arguments, name_template) in zip(self.name.name, self.name.positions, self.name.targets):
            name_arguments = [a.create_template_instance(template, arguments, position) for a in name_arguments]
            n = Name(self.lexer, (name,), pos, targets=((None, name_arguments, name_template),))
            if result:
                result += n
            else:
                result = n
        result = DependentTypeName(self.lexer, self.position, result)
        return result

    def _create_template_instance(self, template, arguments, position):
        current_object = None
        for name, name_position, (_, name_arguments, _) in zip(self.name.name, self.name.positions, self.name.targets):
            current_object = self.resolve(current_object, name, name_position, name_arguments, template, arguments, position)
            if not current_object:
                return self._create_partial_template_instance(template, arguments, position)
        return current_object

    def _distance(self, other, matches, typeref, other_typeref, allowed_cast):
        if isinstance(other, DependentTypeName):
            # TODO
            if self.name != other.name:
                if allowed_cast == CAST_UNRELATED:
                    d = Type.Distance(variant = -1)
                    return d.match_attributes(allowed_cast, typeref.qualifiers, other_typeref.qualifiers)
                else:
                    raise CastError()
            else:
                d = Type.Distance(cast=0)
                return d.match_attributes(allowed_cast, typeref.qualifiers, other_typeref.qualifiers)
        elif allowed_cast == CAST_UNRELATED:
            d = Type.Distance(variant = -1)
            return d.match_attributes(allowed_cast, typeref.qualifiers, other_typeref.qualifiers)
        else:
            raise CastError()


class TypeRef(CppObject):
    def __init__(self, lexer, position, type):
        CppObject.__init__(self, lexer, position)
        self.type = type
        self.qualifiers = set([])

    def get_type(self):
        return self.type.get_type()

    def is_void(self):
        return isinstance(self.type.get_type(), Void)

    def add_qualifier(self, qualifier):
        self.qualifiers.add(qualifier)

    def clone(self):
        result = TypeRef(self.lexer, self.position, self.type)
        for q in self.qualifiers:
            result.qualifiers.add(q)
        return result

    def distance(self, other, allowed_cast, matches = {}):
        if isinstance(self.type, TypeDef):
            return self.type.type.distance(other, allowed_cast, matches)
        elif isinstance(other, TypeRef):
            if isinstance(other.type, TypeDef):
                return self.distance(other.type.type, allowed_cast, matches)
            else:
                return self.type._distance(other.type, matches, self, other, allowed_cast)
        else:
            raise CastError()

    def _create_template_instance(self, template, arguments, position):
        type = self.type.create_template_instance(template, arguments, position)
        if isinstance(type, TypeRef):
            result = type.clone()
            result.qualifiers |= self.qualifiers
        else:
            result = self.clone()
            result.type = type
        return result

    def __str__(self):
        return str(self.type) + (self.qualifiers and ' '+' '.join(self.qualifiers) or '')
    __repr__ = __str__

    def get_unresolved_parameters(self):
        return self.type.get_unresolved_parameters()

    def signature(self):
        s = self.type.signature()
        for a in self.qualifiers:
            if a == 'const':
                s = 'K'+s
            elif a == 'volatile':
                s = 'V'+s
            else:
                assert False
        return s
