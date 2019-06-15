from .cppobject import CppObject
from .scope import Scope


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
    def __init__(self, owner, parent):
        Scope.__init__(self, owner)
        self.parent = parent
        self.destructor = None


class Struct(Type):
    global_index = 0

    def __init__(self, lexer, position, struct_type, name):
        Type.__init__(self, lexer, position, name)
        self.struct_type = struct_type
        self.index = self.global_index
        self.global_index += 1

    def get_token_type(self):
        return 'STRUCT_ID'

    def define(self, parent):
        self.push_scope(StructScope(self, parent))

    def _create_template_instance(self, template, arguments, position):
        return Struct(self.lexer, self.position, self.struct_type, self.name)

    def _complete_template_instance(self, result, template, arguments, position):
        if self.scope:
            result.define(self.scope.parent and self.scope.parent.create_template_instance(template, arguments, position))
            self.scope.create_template_instance(result.scope, template, arguments, position)
            self.lexer.pop_scope()
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


class TypeRef(CppObject):
    def __init__(self, lexer, position, type):
        CppObject.__init__(self, lexer, position)
        self.type = type
        self.qualifiers = set([])

    def add_qualifier(self, qualifier):
        self.qualifiers.add(qualifier)

    def clone(self):
        result = TypeRef(self.lexer, self.position, self.type)
        for q in self.qualifiers:
            result.qualifiers.add(q)
        return result

    def distance(self, other, allowed_cast, matches = {}):
        if isinstance(other, TypeRef):
            return self.type._distance(other.type, matches, self, other, allowed_cast)
        else:
            raise CastError()

    def _create_template_instance(self, template, arguments, position):
        result = self.clone()
        result.type = result.type.create_template_instance(template, arguments, position)
        return result

    def __str__(self):
        return str(self.type) + (self.qualifiers and ' '+' '.join(self.qualifiers) or '')
    __repr__ = __str__

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
